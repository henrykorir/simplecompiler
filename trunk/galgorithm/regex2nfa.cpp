#include "regex2nfa.h"
#include <iostream>
#include <deque>
#include <vector>
#include <list>

#include <stringX.h>

using namespace compile;
using namespace compile::ga;

#define FIRST_NONTSID 129

struct parsecontent
{
	parsecontent(const char* p)
		: iS(0)
		, s(p)
	{}

	int brace_stage(int L, char rc/*, int& end*/)
	{
		int vtmp;
		std::vector<int> v;
		const char* p = s;
		int rcode = 0;
		while(*s/* && *s != rc*/)
		{
			while(*s != '\0' && *s != '(' && *s != ')' && *s != '|') ++s;
			if(*s && !isrealsep(p, s))
			{
				++ s;
			}
			else if(*s == '\0' || *s == rc)
			{
				if(p != s)
				{
					make_node(p, s, vtmp = ++iS);
					v.push_back(vtmp);
				}
				p = s + 1;
				if(*s == rc) { rcode = 1; ++ s; break; }
			}
			else 
			{
				if(switch_brace_or(v, p, L, rc) == 1)
				{
					rcode = 1;
					break;
				}
				p = s;
			}
		}

		insert_new_prod(L, &v[0], v.size(), p_seq);
		return rcode;
	}

	int switch_brace_or(std::vector<int>& R, const char* p, int L, char rc)
	{
		int vtmp = -1;
		if(p != s)
		{
			make_node(p, s, vtmp = ++iS);
			R.push_back(vtmp);
		}
		switch(*(s ++))
		{
		case '(':
			{
				switch(brace_stage(vtmp = ++iS, ')'))
				{
				case 1: // found matched ')'
					R.push_back(vtmp);
					if(vtmp = isrepat(s)) 
					{
						int tmpR[2] = {R.back(), vtmp};
						insert_new_prod(vtmp = ++iS, tmpR, 2, p_rep);
						R.back() = vtmp;
					}
					break;
				case -1: // empty
					throw std::runtime_error("no matched )");
				default:
					throw std::runtime_error("no matched )");
				}
			}
			break;
		case '|':
			{
				if(R.empty()) throw std::runtime_error("error no item before '|'");
				int vL = ++iS;
				insert_new_prod(vL, &R[0], R.size(), p_seq);
				R.clear();
				R.push_back(vL);
				int vr = brace_stage(vtmp = ++iS, rc);
				R.push_back(vtmp);
				insert_new_prod(vtmp = ++iS, &R[0], R.size(), p_or);
				R.clear();
				R.push_back(vtmp);
				
				return vr;
			}
		}
		return 0;
	}

	int isrepat(const char*& s)
	{
		if(*s == '+' || *s == '*' || *s == '?')
		{
			int vtmp = *s; // just using asscii
			const char* t = s ++;
			//make_symbol(std::string(1, *t), vtmp = ++iS);
			return vtmp;
		}
		return 0;
	}

	bool isrealsep(const char* p, const char* s)
	{
		const char* t = s - 1;
		while(t != p - 1 && *t == '\\') --t;
		return (s - t - 1) % 2 == 0; // true brace
	}

	// we assume '|' never in [], but '\|' can in []
	// 	and also no '++' or '+*' ...
	void make_node(const char* vs, const char* ve, int L)
	{
		const char* ps = vs;
		do {while(ps != ve && *ps != '[') ++ps;}
		while(!isrealsep(vs, ps));
		const char* pe = ps == ve ? ve : (ps + 1);
		do {while(pe != ve && *pe != ']') ++ pe;}
		while(!isrealsep(ps, pe));
		if(ps == ve && pe == ve) 
		{
			true_node(vs, ve, L);
		}
		else if(ps != ve && pe != ve)
		{
			int R[4]; int c = 0;
			if(ps != vs) true_node(vs, ps, R[c++] = ++iS);
			if(ps != pe) 
			{
				R[c ++] = ++ iS;
				true_node(ps + 1, pe, R[c] = ++iS);
				insert_new_prod(R[c-1], &R[c], 1, p_switch);
			}
			if(pe + 1 != ve) 
			{
				const char* t = pe + 1;
				if(R[c] = isrepat(t))
				{
					if(c == 0) throw std::runtime_error("no charator before +*?");
					int vtmp = ++iS;
					insert_new_prod(vtmp, R + c - 1, 2, p_rep);
					R[c-1] = vtmp;
				}
				
				if(t != ve) make_node(t, ve, R[c++] = ++iS);
			}
			insert_new_prod(L, R, c, p_seq);
		}
		else throw std::runtime_error("not matched []");
	}

	void true_node(const char* vs, const char* ve, int L)
	{
		const char* p = vs;
		std::string x;
		bool isskip = false;
		while((isskip || (*p != '*' && *p != '+' && *p != '?')) && p != ve)
		{
			if (*p == '\\') 
			{
				isskip = !isskip;
				if(!isskip) x.push_back(*p);
			}
			else 
			{
				x.push_back(*p);
				isskip = false;
			}
			++ p;
		}
		if(ve == p)
		{
			make_symbol(x, L);
		}
		else
		{
			if(x.empty()) throw std::runtime_error("no charactor before +*?");
			int R[3]; int c = 0;
			char ch = x[x.size() - 1];
			x.resize(x.size() - 1);
			if(!x.empty()) make_symbol(x, R[c++] = ++iS);
			R[c] = ++iS;
			R[c + 1] = ++iS;
			
			make_symbol(std::string(1, ch), R[c]);
			make_symbol(std::string(1, *p), R[c + 1]);
			int vL = ++iS;
			insert_new_prod(vL, R+c, 2, p_seq);
			R[c++] = vL;
			if(p + 1 != ve) true_node(p + 1, ve, R[c++] = ++iS);
			insert_new_prod(L, R, c, p_seq);
		}
	}

	void make_symbol(const std::string& x, int L)
	{
		sym asym;
		asym.sid = L;
		asym.s = x;
		syms.push_back(asym);
	}

	void insert_new_prod(int L, const int* R, int c, int prod_type)
	{
		prod p;
		p.L = L;
		p.iR = (int)buf.size();
		p.nR = c;
		p.pt = prod_type;
		buf.insert(buf.end(), R, R + c);
		prods.push_back(p);
	}

	enum {p_or, p_switch, p_seq, p_rep, p_brace};
	// simple production
	struct prod
	{
		int L; // left
		int iR; // first right index
		int nR; // right count
		int pt; // production type: p_or, p_switch, p_seq
	};

	// simple symbol
	struct sym
	{
		int sid;
		std::string s;
	};

	int getsymbol(int sid)
	{
		for(int i = 0; i < syms.size(); ++ i)
		{
			if(syms[i].sid == sid) return i;
		}
		throw std::runtime_error("can't find speical symbol");
	}

	struct list
	{
		int n;
		list* next;
	};

	list* new_list(list* pnext = NULL, int n = 0)
	{
//		buf.resize(buf.size() + 1);
//		buf.back().next = pnext;
//		buf.back().n = n;
//		return &buf.back();
		return NULL;
	}

	void insert(list* ph, int v)
	{
		list* p = new_list();
		p->next = ph->next;
		p->n = v;
		ph->next = p;
	}

	const char* s;
	//std::deque<list> buf;
	std::deque<int> buf;
	std::deque<prod> prods;
	std::deque<sym> syms;
	int iS;
};

void set_grammar_symbol(symholder& sholder, parsecontent& pc)
{
	// first FIRST_NONTSID symbols are one charator symbol
	// 	sholder[0]: epsilon
	// 	sholder[FIRST_NONTSID-1]: ending symbol
	// 	sholder[FIRST_NONTSID~pc.iS]: non-terminate symbol
	const int nchs = FIRST_NONTSID - 1;
	std::list<std::string> dstr;
	std::deque<symbol> ss(pc.iS + 1 + FIRST_NONTSID);

	// make single charator symbol, we just use 0-127 now
	for(int i = 0; i < nchs; ++ i)
	{
		dstr.push_back(std::string(1, (char)i));
		ss[i].name = dstr.back().c_str();
		ss[i].Lfuncs = 0;
		ss[i].funcs = NULL;
		ss[i].Lname = (int32)dstr.back().size();
		ss[i].sid = i;
		ss[i].ist = 1;
	}
	// ending symbol
	dstr.push_back(std::string(1, (char)-1));
	ss[nchs] = ss[nchs-1];
	ss[nchs].name = dstr.back().c_str();
	ss[nchs].sid = nchs;

	// non-terminate symbol
	for(int i = FIRST_NONTSID; i <= pc.iS; ++ i)
	{
		dstr.push_back(stringX::format("S%d", i));
		ss[i].name = dstr.back().c_str();
		ss[i].Lfuncs = 0;
		ss[i].funcs = NULL;
		ss[i].Lname = (int32)dstr.back().size();
		ss[i].sid = i; //pc.syms[i - nchs].sid;
		ss[i].ist = 0;	
	}
	{
		symholder newholder(ss.begin(), ss.end());
		newholder.swap(sholder);
		sholder.make_index();
	}
}

void set_grammar_production(tinygrammar::vecprods& productions, parsecontent& pc, int gending)
{
//	const int nchs = FIRST_NONTSID;
	std::deque<production> tmpprods;
	
	kog::smart_vector<char> isc(pc.iS + 1);
	memset(isc.get(), -1, isc.size_in_bytes());
	for(size_t i = 0; i < pc.prods.size(); ++ i)
	{
		parsecontent::prod p = pc.prods[i];
		if(p.pt == parsecontent::p_switch)
		{
			isc[pc.buf[p.iR]] = 0;
		}
	}
	// first to make all symbol production
	std::vector<int32> ending(pc.iS + 1, -1);
	for(size_t i = 0; i < pc.syms.size(); ++ i)
	{
		if(!isc[pc.syms[i].sid]) continue;
		const std::string& s = pc.syms[i].s;
		int L = pc.syms[i].sid;
		int R[2] = {0, 0};
		for(size_t j = 0; j < s.size(); ++ j)
		{
			R[0] = s[j]; R[1] = ++ pc.iS;
			tmpprods.push_back(production(L, R, 2));
			L = R[1];
		}
		ending[pc.syms[i].sid] = R[1];
	}
	ending[FIRST_NONTSID] = gending; 
	for(size_t i = 0; i < pc.prods.size(); ++ i)
	{
		if(ending[pc.prods[i].L] == -1) ending[pc.prods[i].L] = ++ pc.iS;
	}
	for(size_t i = 0; i < pc.prods.size(); ++ i)
	{
		parsecontent::prod p = pc.prods[i];
		switch(p.pt)
		{
		case parsecontent::p_or:
			for(int x = 0; x < p.nR; ++ x)
			{
				tmpprods.push_back(production(p.L, &pc.buf[p.iR + x], 1));
				tmpprods.push_back(production(ending[pc.buf[p.iR + x]], &ending[p.L], 1));
			}
			break;
		case parsecontent::p_seq:
			{
				int L = p.L;
				for(int x = 0; x < p.nR; ++ x)
				{	
					tmpprods.push_back(production(L, &pc.buf[p.iR + x], 1));
					L = ending[pc.buf[p.iR + x]];
				}
				tmpprods.push_back(production(L, &ending[p.L], 1));
			}
			break;
		case parsecontent::p_rep:
			switch((char)pc.buf[p.iR + 1]) // just a charator, so it must be symbol index
			{
			case '+':
				{
					int R[2] = {pc.buf[p.iR], ending[p.L]};
					// S->a
					tmpprods.push_back(production(p.L, R, 1));
					tmpprods.push_back(production(ending[R[0]], &ending[p.L], 1));
					// S->aS
					tmpprods.push_back(production(ending[R[0]], &p.L, 1));
				}
				break;
			case '*':
				{
					//int e = 0; //symholder::epsilon;
					tmpprods.push_back(production(p.L, &ending[p.L], 1)); // S->epsilon
					tmpprods.push_back(production(p.L, &pc.buf[p.iR], 1)); // S->aS
					tmpprods.push_back(production(ending[pc.buf[p.iR]], &p.L, 1));
				}
				break;
			case '?':
				{
					tmpprods.push_back(production(p.L, &ending[p.L], 1)); // S->epsilon
					tmpprods.push_back(production(p.L, &pc.buf[p.iR], 1)); // S->a
					tmpprods.push_back(production(ending[pc.buf[p.iR]], &ending[p.L], 1));
				}
				break;
			default:
				throw std::runtime_error("invalidate production!");
			}
			break;
		case parsecontent::p_switch:
			{
				const std::string&s = pc.syms[pc.getsymbol(pc.buf[p.iR])].s;
				if(s.empty()) throw std::runtime_error("invalidate switch");
				char isEx = s[0] == '^' ? 0 : 1;
				char inlist[128]; // we just use charators(0~127)
				memset(inlist, 0, sizeof(inlist));
				for(size_t x = 1 - isEx; x < s.size(); )
				{
					if(x + 1< s.size() && s[x + 1] == '-')
					{// range
						if(x + 2 >= s.size()) throw std::runtime_error("invalidate range");
						for(char ch = s[x]; ch <= s[x+2]; ++ ch)
						{
							inlist[ch] = 1;
						}
						x += 2;
					}
					else
					{
						inlist[s[x]] = 1;
						++ x;
					}
				}
				int R[2] = {0, ending[p.L]};
				for(int x = 0; x < 128; ++ x)
				{
					if(inlist[x] == isEx && std::isprint(x)) 
					{
						R[0] = x;
						tmpprods.push_back(production(p.L, R, 2));
					}
				}
			}
			break;
		default:
			throw std::runtime_error("invalidate type!");
		}
	}
	productions.reset(tmpprods.size());
	std::copy(tmpprods.begin(), tmpprods.end(), productions.begin());
}

void regex2nfa::operator()(const tstring& input, grammar& otput)
{
	tocfg(input, otput);
//	tonfa(otput, otput);
}
	
void regex2nfa::tocfg(const tstring& input, grammar& otput)
{
	tinygrammar tmpg;
	parsecontent pc(input.c_str());
	const int nchs = 128;

	std::cout<<input<<std::endl;
	pc.iS = FIRST_NONTSID;
	pc.brace_stage(FIRST_NONTSID, '\0');

	int S_ending = nchs;
	set_grammar_production(tmpg.productions(), pc, S_ending);
	set_grammar_symbol(tmpg.symbols(), pc);
	tmpg.starts() = FIRST_NONTSID;
	tmpg.endings() = S_ending;
	tmpg.eplisons() = 0;

	grammar newG(tmpg);
	otput.swap(newG);
}

void regex2nfa::tonfa(const grammar& input, grammar& otput)
{
}
