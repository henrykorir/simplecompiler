#include "regex2nfa.h"
#include <iostream>
#include <deque>
#include <vector>
#include <list>
#include <cctype>
#include <stack>
#include <queue>

#include <stringX.h>
#include <scerror.h>
#include <logger.h>
#include <indentstream.h>

using namespace compile;
using namespace compile::ga;

#define LOG_REGEX_TO_NFA

#ifdef LOG_REGEX_TO_NFA
#include <logger.h>
#include "../test/gio.h"
#endif

// can't enable this macro
//#define JUST_PRINTABLE_CHAR

struct parsecontent
{
	NON_COPYABLE_OBJECT(parsecontent);
public:
	parsecontent(const char* p)
		: rs(p)
		, len(0)
	{
		while (*(p ++)) ++len;
		maine = empty_elem();
		maine->flag = eroot;
		maine->repmin = maine->repmax = 1;
		ecur = maine;
	}

	~parsecontent()
	{
		// free all content
		std::stack<elem*> mstack;
		mstack.push(maine);
		while(!mstack.empty())
		{
			elem* m = mstack.top();
			mstack.pop();
			if (m->flag != emeta)
			{
				elem* c = m->first;
				while (c != NULL)
				{
					mstack.push(c);
					c = c->next;
				}
			}
			alloc.deallocate(m, 1);
		}
		maine = ecur = NULL;
	}

	bool start_parse()
	{
		const char* s = rs;
		try{
			while (*s != '\0')
			{
				switch_ch(s, 0);
				++ s;
			}
			if (ecur != maine) fire("have unmatched brace '[' or '('");
			// split global or
			split_or();
			make_sure_root();
		}catch (std::exception& ex) {
			logerror(ex.what());
			return false;
		}catch (...){
			logerror("some unknown error!");
			return false;
		}
		return true;
	}

	void switch_ch(const char*& s, int32 flag)
	{
		switch (*s)
		{
		case '\\': 
			parse_inv(s, flag);
			break;
		case '.':
			new_meta(any_char);
			break;
		case '+':
			check_canset_repeat(s);
			ecur->last->repmin = 1;
			ecur->last->repmax = nolimits;
			ecur->last->repsetted = true;
			break;
		case '*':
			check_canset_repeat(s);
			ecur->last->repmin = 0;
			ecur->last->repmax = nolimits;
			ecur->last->repsetted = true;
			break;
		case '?':
			check_canset_repeat(s);
			ecur->last->repmin = 0;
			ecur->last->repmax = 1;
			ecur->last->repsetted = true;
			break;
		case '|':
			// make last one and this as switch
			if (ecur->last == NULL) fire("invalidate or '|' @%d", s - rs);
			// split current
			new_meta(or_flag);
			break;
		case '(':
			new_group();
			break;
		case ')':
			if (outof_group() == NULL)
				fire("unmatched ')' @%d", s - rs);
			break;
		case '[':
			new_switch();
			parse_switch(s);
			break;
		case ']':
			fire("invalidate ] here @%d", s - rs);
			break;
		case '{':
			check_canset_repeat(s);
			// start read repeat count
			repeat_count(s);
			break;
		//case '}': repeat_count have read '}'
		case '$':
		case '^':
			fire("don't support %c!", s[0]);
			break;
		default:
			new_meta(s[0]);
			break;
		}
	}

	void repeat_count(const char*& s)
	{
		const char*& p = ++s ; // current is '{'
		// skip white spaces
		//while (*p == ' ' || *p == '\t') ++ p;
		const char* minps = p;
		// {[0-9]+,[0-9]*} | {[0-9]+}
		while (*p <= '9' && *p >= '0') ++p;

		if (*p == '\0') fire("invalidate repeat count at end of the string");
		else if (*p == '}') // {[0-9]+}
		{
			// finish read count
			if (p == minps) fire("repeat count don't have any digits @%d", p - rs);
			ecur->last->repmin = ecur->last->repmax = str2int32(minps, p);
		}
		else if (*p == ',') // {[0-9]+,[0-9]*}
		{
			if (p == minps) fire("min repeat count don't have any digits @%d", p - rs);
			// set min repeat count
			ecur->last->repmin = str2int32(minps, p);
			ecur->last->repmax = nolimits;
			// start to read max count
			++ p; // eat ','
			//while (*p == ' ' || *p == '\t') ++ p; // skip whitesapce
			const char* maxps = p;
			while (*p <= '9' && *p >= '0') ++ p;
			if (*p == '\0') fire("invalidate repeat count at end of the string, @%d", p - rs);
			else if (*p == '}')
			{
				if (p == maxps) ; // no max repeat count
				else 
				{
					ecur->last->repmax = str2int32(maxps, p);
				}
			}
			else 
			{
				// skip ending white spaces before '}'
				const char* maxpe = p;
				//while (*p == ' ' || *p == '\t') ++ p;
				if (*p != '}') fire("invalidate max repeat count @%d", p - rs);
				else
				{
					ecur->last->repmax = str2int32(maxps, maxpe);
				}
			}
		}
		else
		{
			fire("invalidate repeat count @%d", p - rs);
		}
		
		ecur->last->repsetted = true;
	}

	void parse_inv(const char*& s, int32 flag)
	{
		if (s[1] == '\0') fire("invalidate '\\' at end of the string");
		switch (s[1])
		{
		case '\\':
		case '+':
		case '*':
		case '?':
		case '(':
		case ')':
		case '[':
		case ']':
		case '{':
		case '}':
		case '|':
		case '$':
		case '^':
		case '.':
			new_meta(s[1]);
			break;
		case 'n':
			new_meta('\n');
			break;
		case 't':
			new_meta('\t');
			break;
		case 'r':
			new_meta('\r');
			break;
		case 'f':
			new_meta('\f');
			break;
		case 'v':
			new_meta('\v');
		case 'w':
			new_meta(word);
			break;
		case 'd':
			new_meta(digits);
			break;
		case 's':
			new_meta(spaces);
			break;
		case 'b':
			fire("don't support \\b now!");
			new_meta(seperator);
			break;
		case 'W':
			new_meta(not_word);
			break;
		case 'S':
			new_meta(not_spaces);
			break;
		case 'D':
			new_meta(not_digits);
			break;
		case 'B':
			fire("don't support \\B now!");
			new_meta(not_seperator);
			break;
		default:
			fire("invalidate %c @%d", s[1], s - rs);
		}
		// eat one charator
		++s;
	}

	void parse_switch(const char*& s)
	{
		++ s; // eat '['
		if (*s == '^')
		{
			// exclude
			ecur->flag = eswitchnot;
			++ s;
		}
		while (*s)
		{
			switch (*s)
			{
			case '\\':
				{
					switch (s[1])
					{
					case '\0':
						fire("invlaidate \\ @%d", s - rs);
						break;
					case ']':
					case '-':
					case '[':
					case '\\':
						new_meta(s[1]);
						break;
					case 'n':
						new_meta('\n');
						break;
					case 't':
						new_meta('\t');
						break;
					case 'r':
						new_meta('\r');
						break;
					case 'f':
						new_meta('\f');
						break;
					case 'v':
						new_meta('\v');
						break;
					case 'w':
						new_meta(word);
						break;
					case 'd':
						new_meta(digits);
						break;
					case 's':
						new_meta(spaces);
						break;
					case 'S':
						new_meta(not_spaces);
						break;
					case 'W':
						new_meta(not_word);
						break;
					case 'D':
						new_meta(not_digits);
						break;
					default:
						fire("invalidate \\ @%d", s - rs);
					}
					++s; // eat a charactor
				break;
				}
			case ']':
				if(NULL == outof_switch())
					fire("no matched '[' @%d", s - rs);
				return;
			case '-':
				{
					if (ecur->last == NULL) fire("invalidate range @", s - rs);
					else if(ecur->last->flag != emeta ||
						!((ecur->last->meta <= '9' && ecur->last->meta >= '0') ||
							(ecur->last->meta <= 'z' && ecur->last->meta >= 'a') ||
							(ecur->last->meta <= 'Z' && ecur->last->meta >= 'A')))
							fire("invalidate range begin @%d", s - rs - 1);
					logstring("expand range");
					int32 meta_from = ecur->last->meta;
					// read next charactor
					if (s[1] <= '9' && s[1] >= '0') //digits
					{
						if (!(meta_from <= '9' && meta_from >= '0'))
							fire("unmatched range @%d", s - rs);
						int diff = s[1] - meta_from;
						if (diff == 0) break; // don't add new charactor
						int ds = diff / abs(diff);
						for (int i = meta_from + ds; i != s[1] + ds; i += ds)
							new_meta(i);
					}
					else if(s[1] <= 'z' && s[1] >= 'a') // lower word
					{
						if (!(meta_from <= 'z' && meta_from >= 'a'))
							fire("unmatched range @%d", s - rs);
						int diff = s[1] - meta_from;
						if (diff == 0) break; // don't add new charactor
						int ds = diff / abs(diff);
						for (int i = meta_from + ds; i != s[1] + ds; i += ds)
							new_meta(i);
					}
					else if(s[1] <= 'Z' && s[1] >= 'A') // lower word
					{
						if (!(meta_from <= 'Z' && meta_from >= 'A'))
							fire("unmatched range @%d", s - rs);
						int diff = s[1] - meta_from;
						if (diff == 0) break; // don't add new charactor
						int ds = diff / abs(diff);
						for (int i = meta_from + ds; i != s[1] + ds; i += ds)
							new_meta(i);
					}
					else fire("invalidate range end charactor @%d", s - rs);
					++ s;
					break;
				}
			case '\0':
				fire("unmatched '[' @%d", s - rs);
				break;
			default:
				new_meta(*s);
				break;
			}
			++s;
		}
	}

	enum
	{
		emeta = 0x01,
		eswitch = 0x02,
		egroup = 0x04,
		eor = 0x08,
		evirg = 0x10,
		eroot = 0x20,
		eswitchnot = 0x40,
	};

	enum 
	{		
		digits = -1,
		word = -2,
		spaces = -3,
		not_digits = -4,
		not_word = -5,
		not_spaces = -6,
		seperator = -7,
		not_seperator = -8,
		any_char = -9,
		or_flag = -10,
	};

	enum 
	{
		nolimits = -1, // *
	};

	struct elem
	{
		union
		{
			struct
			{
				elem* first; // first child of group/switch
				elem* last; // last child of group/switch
			};
			int32 meta; // current meta value
		};
		elem* parent; // parent group
		elem* next;
		bool repsetted; // have set repeat time
		uint32 repmin; // min repeat times
		uint32 repmax; // max repeat times
		uint32 flag;  // first or meta, which is valid?
	};

	elem* new_switch()
	{
		elem* p = empty_elem();
		p->flag = eswitch;
		p->repmin = 1;
		p->repmax = 1;
		p->parent = ecur;
		insert(p);
		ecur = p;
		logstring("new switch");
		return p;
	}

	elem* new_group()
	{
		elem* p = empty_elem();
		p->flag = egroup;
		p->repmin = 1;
		p->repmax = 1;
		p->parent = ecur;
		insert(p);
		ecur = p;
		logstring("new group");
		return p;
	}

	elem* new_meta(int32 meta)
	{
		elem* p = empty_elem();
		p->meta = meta;
		p->flag = emeta;
		p->repmin = 1;
		p->repmax = 1;
		p->parent = ecur;
		insert(p);

		if (meta > 0) logstring("new meta %c(%d)", (char)meta, meta);
		else logstring("new meta flag[%d]", meta);
		return p;
	}

	elem* empty_elem()
	{
		elem* p = alloc.allocate(1);
		memset(p, 0, sizeof(elem));
		return p;
	}

	elem* outof_group()
	{
		logstring("outof group");
		if (ecur == maine) return NULL;
		if (ecur->flag != egroup)
			return NULL;

		// if hava or statement, must split it
		split_or();
		return ecur = ecur->parent;
	}

	elem* outof_switch()
	{
		logstring("outof switch");
		if (ecur == maine)
			return NULL;
		// don't need check eor statement
		if (!(ecur->flag == eswitch || ecur->flag == eswitchnot))
			return NULL;
		return ecur = ecur->parent;
	}

	void split_or()
	{
		elem* c = ecur->first;
		elem* x = NULL; // x->next == c
		while (c != NULL && c->meta != or_flag) {x = c; c = c->next;}
		if (NULL == c) return; // don't hava or(|)
		
		logstring("start split or");
		if (ecur->first->meta == or_flag) fire("or | must not at begin of a group");
		if (ecur->last->meta == or_flag) fire("or | must not at end of a group");

		elem* e = new_meta(or_flag); // using it as last or_elem
		elem* s = ecur->first; // first child of the new group
		ecur->first = ecur->last = NULL;
		while (c != NULL)
		{
			elem* n = c->next; // n is next child
			// use c as new group
			c->flag = egroup;
			c->first = s;
			c->last = x; // x->next == c, so x is last child
			x->next = NULL; // trunk link list
			elem* t = c->first;
			while (t != NULL) // reset children's parent
			{
				t->parent = c;
				t = t->next;
			}
			insert(c);
			
			if (n == NULL) break;

			s = n; // next start child
			c = n;
			x = NULL;
			while (!(c->flag == emeta && c->meta == or_flag)) { x = c; c = c->next; }
			if (x == NULL)
			{
				insert(s);
				fire("| and | can't adjust");
			}
		}
		
		ecur->flag = eor; // set or flag
	}

	elem* mov_last_down()
	{
		elem* p = empty_elem();
		p->flag = egroup;
		p->first = p->last = ecur->last;
		p->repmin = 1;
		p->repmax = 1;
		p->parent = ecur;

		// update last's parent
		ecur->last->parent = p;
		
		// update parent's and brother's link
		if (ecur->first == ecur->last) // just one child
		{
			ecur->first = ecur->last = p;
		}
		else
		{ // have brother, find x: adjust brother
			elem* x = ecur->first;
			while (x->next != ecur->last) x = x->next;
			x->next = p; // reset pre child's next
			ecur->last = p;
		}

		logstring("mov last down");
		
		return p;
	}

	void check_canset_repeat(const char* s)
	{
		if (ecur->last == NULL)
			fire("invalidate repeat count @%d", s - rs);
		if (ecur->repsetted)
		{
			// hava set repeat count
			//  so, must make a new group wrap ecur->last
			mov_last_down();
		}
	}

	void insert(elem* p)
	{
		if (ecur->last != NULL)
		{
			ecur->last->next = p;
			ecur->last = p;
		}
		else
		{
			ecur->first = ecur->last = p;
		}
	}

	int32 str2int32(const char* s, const char* e)
	{
		int32 x = 0;
		while (s != e)
		{
			x = 10 * x + (s[0] - '\0');
			if (x < 0) fire("integer overflow @%d", s - rs);
			++ s;
		}
		return x;
	}

	void make_sure_root()
	{
		if (maine->flag == eor)
		{
			// main can't be or
			elem* p = empty_elem();
			p->flag = eroot;
			p->first = p->last = maine;
			p->repmin = p->repmax = 1;
			maine->parent = p;
			ecur = maine = p;
		}
	}

	std::allocator<elem> alloc;
	elem* ecur;
	elem* maine;
	const char* rs;
	int32 len;
};

void regex2nfa::init_terminates()
{
	// 	sholder[0]: epsilon
	const int32 SUPPORT_CHARACTOR_COUNT = 128;
	// init 
	// first FIRST_NONTSID symbols are one charator symbol
 	// 	sholder[0~SUPPORT_CHARACTOR_COUNT-1]: epsilon
	// 	sholder[SUPPORT_CHARACTOR_COUNT]: epsilon
	//  sholder[SUPPORT_CHARACTOR_COUNT+1]: ending symbol
	
	tmpsyms_.clear();
	//std::vector<char> printable_chars;

	char2sidmap_.resize(SUPPORT_CHARACTOR_COUNT + 2, -1);
	for(int32 i = 0; i < SUPPORT_CHARACTOR_COUNT; ++ i)
	{
#ifdef JUST_PRINTABLE_CHAR
		if (!(std::isprint(i) || i == 0 ||
			i == '\t' || i == '\r' || i == '\n' || i == '\f')) continue;
#endif
		symbol s;
		memset(&s, 0, sizeof(s));
		strbuffer_.push_back(std::string(1, (char)i));
		s.name = strbuffer_.back().c_str();
		s.Lname = (int32)strbuffer_.back().size();
		s.sid = (int32)tmpsyms_.size();
		s.ist = 1;
		tmpsyms_.push_back(s);
		char2sidmap_[i] = s.sid;
	}

	eplison_ = new_nonterm();
	tmpsyms_[eplison_].name = "eplison";
	tmpsyms_[eplison_].Lname = 7;
	tmpsyms_[eplison_].ist = 1; // mark eplison symbol as terminate
	char2sidmap_[SUPPORT_CHARACTOR_COUNT] = eplison_;
	
	eof_ = new_nonterm();
	tmpsyms_[eof_].name = "eof";
	tmpsyms_[eof_].Lname = 3;
	tmpsyms_[eof_].ist = 1; // mark ending symbol as terminate
	char2sidmap_[SUPPORT_CHARACTOR_COUNT+1] = eof_;
	
}

int32 regex2nfa::anychar_sid(int32 R)
{
	int32 L = new_nonterm();

	int32 Rs[2] = {eplison_, R};
	// add productions: L -> a(, b ...) R
	for (int32 i = 1; i < (int32)tmpsyms_.size(); ++ i)
	{
		if (!tmpsyms_[i].ist) break;
		else if (tmpsyms_[i].name[0] == '\r' || tmpsyms_[i].name[0] == '\n') continue; // skip newline charactor
		Rs[0] = i;
		production p(L, Rs, 2);
		tmpps_.push_back(p);
	}

	return L;
}

int32 regex2nfa::digits_sid(int32 R)
{
	int32 L = new_nonterm();
	int32 Rs[2] = {eplison_, R};

	// add productions: L -> 0(, 1, ...) R
	for (int32 i = '0'; i <= '9'; ++ i)
	{
		Rs[0] = char2sidmap_[i];
		production p(L, Rs, 2);
		tmpps_.push_back(p);
	}
	return L;
}

int32 regex2nfa::word_sid(int32 R)
{
	int32 L = new_nonterm();
	int32 Rs[2] = {eplison_, R};

	// add productions: L -> a(, b, ...z, A, B, ...Z) R
	for (int32 i = 'a'; i <= 'z'; ++ i)
	{
		Rs[0] = char2sidmap_[i];
		production p(L, Rs, 2);
		tmpps_.push_back(p);

		p[0] = char2sidmap_[i - 'a' + 'A'];
		tmpps_.push_back(p);
	}

	return L;
}

int32 regex2nfa::spaces_sid(int32 R)
{
	int32 L = new_nonterm();
	int32 Rs[2] = {eplison_, R};

	const std::string spaces = " \t\v";
	// add productions: L -> spaces[i] R
	foreach (int32 i, spaces.begin(), spaces.end())
	{
		Rs[0] = char2sidmap_[i];
		production p(L, Rs, 2);
		tmpps_.push_back(p);
	}
	return L;
}

int32 regex2nfa::new_nonterm()
{
	int32 sid = (int32)tmpsyms_.size();
	symbol s;
	memset(&s, 0, sizeof(s));
	strbuffer_.push_back(stringX::format("S%d", sid));
	s.name = strbuffer_.back().c_str();
	s.Lname = (int32)strbuffer_.back().size();
	s.sid = sid;
	s.ist = 0; // mark word as non-terminate symbol
	tmpsyms_.push_back(s);
	return s.sid;
}

void regex2nfa::operator()(const tstring& input, tinygrammar& otput)
{
	if (is_direct_)
	{
		todfa_direct(input, otput);
	}
	else
	{
		tocfg(input, otput);
	//	tonfa(otput, otput);
	}
#ifdef LOG_REGEX_TO_NFA
	logstring("[regex2nfa] result");
	gwriter gw(kog::loggermanager::instance().get_logger().getos());
	gw<<otput;
#endif
}

int32 regex2nfa::make_ps(int32 L, int32 meta, int32 R)
{
	int32 x = (R == -1 ? new_nonterm() : R);

	if (meta >= 0)
	{
		int32 Rs[2] = {char2sidmap_.at(meta), x};
		tmpps_.push_back(production(L, Rs, 2));
	}
	else if (meta == parsecontent::any_char)
	{
		make_ps(L, eplison_, anychar_sid(x));
	}
	else if(meta == parsecontent::digits)
	{
		make_ps(L, eplison_, digits_sid(x));
	}
	else if (meta == parsecontent::word)
	{
		make_ps(L, eplison_, word_sid(x));
	}
	else if (meta == parsecontent::spaces)
	{
		make_ps(L, eplison_, spaces_sid(x));
	}
	else fire("invalidate meta(%d)", meta);
	
	return x;
}

void print_content(const parsecontent& pc);

struct proccess_content
{
	int32 L;
	std::queue<kog::triple<const parsecontent::elem*, int32, int32> > *mqueue;
	const parsecontent::elem* c;
};

int32 regex2nfa::make_cfg_forelem(proccess_content& pc)
{
	const parsecontent::elem* c = pc.c;
	int32 L = pc.L;
	std::queue<kog::triple<const parsecontent::elem*, int32, int32> >& mqueue = *pc.mqueue;

	switch (c->flag)
	{
	case parsecontent::emeta:
		L = make_ps(L, c->meta);
		break;
	case parsecontent::eroot:
		fire("can't be root here!");
		break;
	case parsecontent::egroup:
		{
			L = make_ps(L, eplison_);
			int32 R = new_nonterm();
			mqueue.push(kog::make_triple(c, L, R));
			L = R;
		}
		break;
	case parsecontent::eor:
		{
			L = make_ps(L, eplison_);
			int32 R = new_nonterm();
			const parsecontent::elem* oc = c->first;
			while (oc != NULL)
			{
				if (oc->flag != parsecontent::egroup) fire("invalidate or");
				int32 x = make_ps(L, eplison_);
				mqueue.push(kog::make_triple(oc, x, R));
				oc = oc->next;
			}
			L = R;
		};
		break;
	case parsecontent::eswitch:
		{
			L = make_ps(L, eplison_);
			int32 R = new_nonterm();
			const parsecontent::elem* oc = c->first;
			while (oc != NULL)
			{
				if (oc->flag != parsecontent::emeta) fire("invalidate switch");
				make_ps(L, oc->meta, R);
				oc = oc->next;
			}
			L = R;
		}
		break;
	case parsecontent::eswitchnot:
		{
			L = make_ps(L, eplison_);
			int32 R = new_nonterm();
			// get exclude charactors
			const int32 SUPPORT_CHARACTOR_COUNT = 128;
			std::vector<int32> excludes(SUPPORT_CHARACTOR_COUNT, 1);
#ifdef JUST_PRINTABLE_CHAR
			for (int32 j = 0; j < SUPPORT_CHARACTOR_COUNT; ++ j)
				if (!std::isprint(j)) excludes[j] = 0;
#endif
			// exclude \r\n
			excludes['\r'] = 0;
			excludes['\n'] = 0;
			const parsecontent::elem* oc = c->first;
			while (oc != NULL)
			{
				int32 r = oc->meta;
				if (r == parsecontent::digits)
				{
					for (int32 e = '0'; e <= '9'; ++ e) 
						excludes[e] = 0;
				}
				else if (r == parsecontent::word)
				{
					for (int32 e = 'a'; e <= 'z'; ++ e)
						(excludes[e] = 0), (excludes[e - 'a' + 'A'] = 0);
				}
				else if (r == parsecontent::spaces)
				{
					//fire("don't support!");
					int32 n = std::min((int32)tmpsyms_.size(), SUPPORT_CHARACTOR_COUNT);
					for (int32 e = 1; e < n; ++ e)
					{
						if (!tmpsyms_[e].ist) break;
#if (defined _WIN32 || defined WIN32)
						else if (std::isspace(e, std::locale(""))) excludes[e] = 0;
#else
						else if (std::isspace(e)) excludes[e] = 0;
#endif
					}
				}
				else if (r < 0 || r >= SUPPORT_CHARACTOR_COUNT)
					fire("invalidate switch char(%d)", r);
				else
				{
					excludes[r] = 0;
				}

				oc = oc->next;
			}
			// make productions
			for (int32 i = 0; i < (int32)excludes.size(); ++ i)
			{
				if (excludes[i] == 0) continue;
				make_ps(L, i, R);
			}
			L = R;
		}
		break;
	default:
		fire("invalidate flag");
		break;
	}
	return L;
}

void regex2nfa::tocfg(const tstring& input, tinygrammar& otput)
{
	parsecontent pc(input.c_str());
	const int nchs = 128;
#ifdef DEBUG_OUTPUT
	logstring("[regex2nfa::tocfg] %s", input.c_str());
#endif
	if(!pc.start_parse()) fire("failed to parse regex string");
	print_content(pc);

	// entry terminates
	init_terminates();

	int32 Start = new_nonterm();
	int32 S_ending = new_nonterm();

	std::queue<kog::triple<const parsecontent::elem*, int32, int32> > mqueue;
	mqueue.push(kog::make_triple(pc.maine, Start, S_ending));
	while (!mqueue.empty())
	{
		kog::triple<const parsecontent::elem*, int32, int32> p = mqueue.front();
		mqueue.pop();

		const parsecontent::elem* m = p.first;
		int32 L = p.second;
		int32 ending = p.third;

		const parsecontent::elem* c = m->first;
		while (c != NULL)
		{
			int32 Lback = L;

			proccess_content prc;
			prc.mqueue = &mqueue;
			prc.c = c;
			prc.L = L;

			L = make_cfg_forelem(prc);

			if (c->repmin == 0) make_ps(Lback, eplison_, L); // ? or *
			if (c->repmax == parsecontent::nolimits) make_ps(L, eplison_, Lback); // +	or *
			
			if (c->repmin != 0 && c->repmin != 1)
			{
				// min is some special value
				for (uint32 i = 1; i < c->repmin; ++ i)
				{
					prc.L = L;
					L = make_cfg_forelem(prc);
				}
			}

			if (c->repmax != 1 && c->repmax != parsecontent::nolimits)
			{
				int32 tx = new_nonterm();
				make_ps(L, eplison_, tx);
				// max is some special value
				for (uint32 i = std::max<uint32>(1, c->repmin); i < c->repmax; ++ i)
				{
					prc.L = L;
					L = make_cfg_forelem(prc);
					make_ps(L, eplison_, tx);
				}
				L = tx;
			}

			c = c->next;
		}
		make_ps(L, eplison_, ending);
	}

	// Ending -> eof
	tmpps_.push_back(production(S_ending, &eof_, 1));

	tinygrammar tmpg(tmpsyms_.begin(), tmpsyms_.end(), 
		tmpps_.begin(), tmpps_.end(), Start, eplison_, eof_);

	otput.swap(tmpg);
}

void regex2nfa::tonfa(const tinygrammar& input, tinygrammar& otput)
{
}

void regex2nfa::todfa_direct(const tstring& input, tinygrammar& otput)
{
	// entry terminates
	init_terminates();

	int32 Start = new_nonterm();
	int32 S_ending = new_nonterm();

	int32 L = Start;
	for (size_t i = 0; i < input.size(); ++ i)
	{
		L = make_ps(L, input[i]);
	}

	make_ps(L, eplison_, S_ending);
	tmpps_.push_back(production(S_ending, &eof_, 1));

	tinygrammar tmpg(tmpsyms_.begin(), tmpsyms_.end(), 
		tmpps_.begin(), tmpps_.end(), Start, eplison_, eof_);

	otput.swap(tmpg);
}

tstring repeat_count(const parsecontent::elem* m)
{
	if (m->repmin == 0 && m->repmax == parsecontent::nolimits)
	{
		return "*";
	}
	else if (m->repmin == 1 && m->repmax == parsecontent::nolimits)
	{
		return "+";
	}
	else if (m->repmax == parsecontent::nolimits)
	{
		return stringX::format("{%d,}", m->repmin);
	}
	else if (!(m->repmax == 1 && m->repmin == 1))
	{
		return stringX::format("{%d,%d}", m->repmin, m->repmax);
	}
	return "";
}

kog::oindentstream& print_pc_meta(kog::oindentstream& os, const parsecontent::elem* m)
{
	switch (m->flag)
	{
	case parsecontent::emeta:
		if (m->meta > 0) os<<(char)m->meta;
		else os<<"["<<m->meta<<"]";
		break;
	case parsecontent::egroup:
		os<<"(group)";
		break;
	case parsecontent::eor:
		os<<"|or|";
		break;
	case parsecontent::eswitch:
		os<<"[switch]";
		break;
	case parsecontent::eswitchnot:
		os<<"[^exclude]";
		break;
	case parsecontent::eroot:
		os<<"{root}";
		 break;
	default:
		fire("invalidate flag");
	}
	os<<repeat_count(m);
	return os;
}

void print_content(const parsecontent& pc)
{
#ifdef LOG_REGEX_TO_NFA
	std::stack<const parsecontent::elem*> mstack;
	mstack.push(pc.maine);
	logstring("print regex prase result content");
	kog::oindentstream os(kog::loggermanager::instance().get_logger().getos());
	while (!mstack.empty())
	{
		const parsecontent::elem* m = mstack.top();
		mstack.pop();
		if (m == NULL) 
		{
			os<<kog::dectab;
			continue; 
		}
		os<<kog::newline;
		print_pc_meta(os, m)<<kog::inctab;
		mstack.push(NULL);
		if (m->flag != parsecontent::emeta)
		{
			std::stack<const parsecontent::elem*> tmpstack;
			m = m->first;
			while (m != NULL)
			{
				tmpstack.push(m);
				m = m->next;
			}

			while (!tmpstack.empty())
			{
				mstack.push(tmpstack.top());
				tmpstack.pop();
			}
		}
	}
	os<<std::endl;
	logstring("parse done");
#endif
}
