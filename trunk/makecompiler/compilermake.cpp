#include "compilermake.h"
#include "gensyntax.h"
#include "prodfunction.h"
#include <stringX.h>
#include <fstream>
#include <scerror.h>
#include <logger.h>
#include <regex/xregex.h>

using namespace compile;
using namespace compile::syntax;

makecompiler::makecompiler(const _Str& syntaxfile)
	: mainfile_(syntaxfile)
	, curfile_(NULL)
{
	curfile_.filename = mainfile_.c_str();
#if (defined _WIN32) || (defined WIN32)
	allfiles_.push_back(stringX::tolower(syntaxfile));
#else
	allfiles_.push_back(syntaxfile);
#endif
	// init quick_replacer
	memset(&starter_, 0, sizeof(starter_));
}

makecompiler::~makecompiler()
{
}

void makecompiler::build_compiler(const _Str& cppfile)
{
	// init current file position
	memset(&curfile_, 0, sizeof(curfile_));

	// parse start main syntax file
	compiler_grammar cgrammar;
	parse_file(mainfile_, cgrammar);

	// generator syntax cpp file
	syntaxgenerator	gensyntax;
	gensyntax(&cgrammar, cppfile, cgrammar.name());
}

size_t makecompiler::parse_file(const std::string& syntaxfile, compiler_grammar& cg)
{
	std::ifstream ifs(syntaxfile.c_str());
	if (!ifs.is_open()) fire("can't open file " + syntaxfile);

	logstring("start to parse file " + syntaxfile);

	fileposition bkfile(syntaxfile.c_str());
	std::swap(bkfile, curfile_);
	
	while (ifs)
	{
		std::string tmp;
		std::getline(ifs, tmp);
		if (!tmp.empty() && tmp[tmp.size()-1] == '\r') tmp.erase(tmp.begin() + tmp.size() - 1); // remove last '\r' at the end of string
		std::string lower_tmp = stringX::tolower(stringX::trim(tmp, 1)); // remove ending spaces
		++ curfile_.iLine;

		if (lower_tmp.empty()) continue; // black line
		else if (lower_tmp[0] == '#') continue; // start with '#', current line is comment
		else if (isspace(lower_tmp[0])) fire(errorpos() + "need not a whitespace at the begin of line");
		else if (lower_tmp == "seprators") parse_seprators(ifs, cg); // add seprators
		else if (lower_tmp == "operators") parse_operators(ifs, cg); // add opreators
		else if (lower_tmp == "includes") parse_includes(ifs, cg); // include a new file
		else if (lower_tmp == "terminates") parse_terms(ifs, cg);
		else if (lower_tmp == "non-terminates") parse_nonterms(ifs, cg);
		else if (lower_tmp == "complex-symbols") parse_complex_symbols(ifs, cg);
		else if (stringX::xregex::is_match("^keywords\\s*:\\s*[^\\s]+\\s*$", lower_tmp)) parse_keywords(ifs, cg, lower_tmp.substr(lower_tmp.find(':') + 1));
		else if (stringX::xregex::is_match("^function\\s*:\\s*[^\\s]+\\s*$", lower_tmp)) parse_function(ifs, cg, lower_tmp.substr(lower_tmp.find(':') + 1));
		else if (lower_tmp.find("->") != std::string::npos) parse_production(ifs, cg, tmp); // add new production
		//else if (lower_tmp == "whitespaces") parse_whitespaces(ifs, cg);
		else if (stringX::xregex::is_match("startsymbol\\s*:.*", lower_tmp)) 
			cg.starts() = cg.index(stringX::trim(tmp.substr(tmp.find(":") + 1)));
		else if (stringX::xregex::is_match("grammarname\\s*:.*", lower_tmp))
			cg.name() = stringX::trim(tmp.substr(tmp.find(":") + 1));
		else 
		{
			fire("invalidate syntax@ %s:%d", syntaxfile.c_str(), curfile_.iLine);
		}
	}
	ifs.close();

	logstring("finish parsed file " + syntaxfile);
	std::swap(curfile_, bkfile);

	return bkfile.iLine;
}

size_t makecompiler::parse_seprators(std::istream& is, compiler_grammar& cg)
{
	logstring("start parse seprators");
	std::string tmp;
	while (get_nextline(is, tmp, curfile_.iLine))
	{
#ifdef WIN32
		if(!std::isspace(tmp[0], std::locale(""))) fire(errorpos() + "failed when read seprators, need a whitespace");
#else
		if(!std::isspace(tmp[0])) fire(errorpos() + "failed when read seprators, need a whitespace");
#endif
		
		tmp = stringX::trim(tmp, 2); // remove leading spaces and ending spaces

		// add these seprators to cg, seperators are represented by digits
		std::vector<tstring> delimiters = stringX::split(tmp, tstring(" \t"), true);
		std::deque<tchar> whitespaces;
		foreach (const tstring& d, delimiters.begin(), delimiters.end())
		{
			int32 x = atoi(d.c_str());
			if (x < 0 || x >= 256) fire(errorpos() + "invalidate delimiter (%d)", x);
			whitespaces.push_back((char)x);
		}
		tstring tmpS(whitespaces.begin(), whitespaces.end());
		cg.make_new_delimiters(tmpS);
	}

	return curfile_.iLine;
}

size_t makecompiler::parse_operators(std::istream& is, compiler_grammar& cg)
{
	std::string tmp;
	while (get_nextline(is, tmp, curfile_.iLine))
	{
#ifdef WIN32
		if(!std::isspace(tmp[0], std::locale(""))) fire(errorpos() + "failed when read opreators, need a whitespace");
#else
		if(!std::isspace(tmp[0])) fire(errorpos() + "failed when read opreators, need a whitespace");
#endif	
		tmp = stringX::trim(tmp, 2); // remove leading spaces and ending spaces
		//_replace(tmp);

		// add this operators to cg
		
		stringX::string_split_t<tchar> spliter(tmp, "\t", false);
		stringX::string_split_t<tchar>::const_iterator iter = spliter.begin();
		operator_t opr;
		opr.txt = stringX::trim(*(iter ++));
		if (opr.txt.empty()) fire(errorpos() + "operator must have txt");

		if (iter != spliter.end())
		{
			opr.name = stringX::trim(*(iter ++));
		}

		// check if exist same operator
		//if (std::find_if(cg.operators().begin(), cg.operators().end(), 
		//			kog::composite_function(kog::mem_value(&operator_t::txt),
		//				std::bind2nd(std::equal_to<tstring>(), opr.txt))) != cg.operators().end())
		//	fire("exist same operator " + opr.txt);
		//cg.operators().push_back(opr);
		cg.make_new_operators(opr.txt, opr.name);
	}

	return curfile_.iLine;
}

size_t makecompiler::parse_keywords(std::istream& is, compiler_grammar& cg, const tstring& basic_symbol)
{
	tstring sname = stringX::trim(basic_symbol);
	int32 symbol_sid = -1; 
	if (!sname.empty())
	{
		symbol_sid = cg.index(sname);
		if (symbol_sid == -1) fire("invalidate baisc symbol %s", sname.c_str());
	}

	std::string tmp;
	while (get_nextline(is, tmp, curfile_.iLine))
	{
#ifdef WIN32
		if(!std::isspace(tmp[0], std::locale(""))) fire(errorpos() + "failed when read opreators, need a whitespace");
#else
		if(!std::isspace(tmp[0])) fire(errorpos() + "failed when read opreators, need a whitespace");
#endif

		// new keywords
		// entry new keyword, 
		cg.make_new_keywords(stringX::trim(tmp), symbol_sid);
	}
	return curfile_.iLine;
}

size_t makecompiler::parse_production(std::istream& is, compiler_grammar& cg, const std::string& prod)
{
	std::string tmp;

	logstring("start to parse production { %s }", prod.c_str());
	size_t rpos = prod.find("->");
	tstring left = stringX::trim(prod.substr(0, rpos));
	std::vector<std::string> rights = stringX::split(prod.substr(rpos + 2), std::string(" "), true);
	// check left and rights
	const tinygrammar& tinyg = cg.gettinyg();
	//if (tinyg.symbols().index(left) == -1 ||
	//		std::find_if(rights.begin(), rights.end(), 
	//			std::bind1st(std::mem_fun_ref(&symholder::index), tinyg.symbols())) != rights.end())
	//	fire("some symbol not found");
	if (tinyg.index(left) == -1) fire(errorpos() + "start symbol (%s) not found", left.c_str());
	foreach (const tstring& r, rights.begin(), rights.end())
		if (tinyg.index(r) == -1) fire(errorpos() + "right symbol (%s) not found", r.c_str());
	
	funcinfo_t pinfo;
	syntax::compiler_action_parser action_parser(pinfo);
	while (get_nextline(is, tmp, curfile_.iLine))
	{
#ifdef WIN32
		if(!std::isspace(tmp[0], std::locale(""))) fire(errorpos() + "failed when read production item, need a whitespace");
#else
		if(!std::isspace(tmp[0])) fire(errorpos() + "failed when read production item, need a whitespace");
#endif

		tmp = stringX::trim(tmp, 2); // remove leading spaces and ending spaces

		if(!action_parser.newline(tmp))
			fire(errorpos() + "invalidate production syntax");
	}

	// insert new production
	cg.make_new_production(left, rights, pinfo);

	return curfile_.iLine;
}

size_t makecompiler::parse_includes(std::istream& is, compiler_grammar& cg)
{
	std::string tmp;
	std::string prepath;
	size_t ppos = mainfile_.find_last_of("\\/");
	if (ppos != std::string::npos)
	{
		prepath = mainfile_.substr(0, ppos + 1);
	}

	while (get_nextline(is, tmp, curfile_.iLine))
	{
#ifdef WIN32
		if(!std::isspace(tmp[0], std::locale(""))) fire(errorpos() + "failed when read includes, need a whitespace");
#else
		if(!std::isspace(tmp[0])) fire(errorpos() + "failed when read includes, need a whitespace");
#endif

		tmp = stringX::trim(tmp); // remove leading spaces and ending spaces

#if (defined _WIN32) || (defined WIN32)
		tmp = stringX::tolower(tmp);
#endif
		tmp = prepath + tmp;
		if (std::find(allfiles_.begin(), allfiles_.end(), tmp) == allfiles_.end())
		{
			allfiles_.push_back(tmp);
			parse_file(tmp, cg);
		}
	}

	return curfile_.iLine;
}

size_t makecompiler::parse_nonterms(std::istream& is, compiler_grammar& cg)
{
	std::string tmp;
	while (get_nextline(is, tmp, curfile_.iLine))
	{
		if (!isspace(tmp[0])) fire(errorpos() + "failed when read non-terms, need a whitespace");

		tmp = stringX::trim(tmp);

		size_t npos = tmp.find(':');
		tstring sname;
		tstring stype;
		if (npos == tstring::npos)
		{
			sname = tmp;
		}
		else
		{
			sname = stringX::trim(tmp.substr(0, npos));
			stype = stringX::trim(tmp.substr(npos + 1));
		}
		// entry symbol
		if (cg.make_new_nonterm_symbol(sname, stype) == -1)
			fire(errorpos() + "invlidate nonterm symbol %s", sname.c_str());
		
	}
	return curfile_.iLine;
}

size_t makecompiler::parse_terms(std::istream& is, compiler_grammar& cg)
{
	std::string tmp;
	while (get_nextline(is, tmp, curfile_.iLine))
	{
		if (!isspace(tmp[0])) fire(errorpos() + "failed when read terms, need a whitespace");

		tmp = stringX::trim(tmp);

		tstring sname;
		tstring regex_string;
		tstring stype;

		std::vector<tstring> ts = stringX::split(tmp, tstring(":"), false);

		sname = stringX::trim(ts[0]);
		if (ts.size() > 1) regex_string = stringX::trim(ts[1]);
		if (ts.size() > 2) stype = stringX::trim(ts[2]);
		
		if (ts.size() > 3) fire(errorpos() + "invalidate terminate string (%s)", tmp.c_str());
		// entry symbol
		if (cg.make_new_term_symbol(sname, regex_string, stype) == -1)
			fire("invlidate term symbol %s", sname.c_str());
		
	}
	return curfile_.iLine;
}

int32 makecompiler::parse_complex_symbol(const tstring& rline, compiler_grammar& cg)
{
	tstring tmp = stringX::trim(rline);
	tstring sname;
	tstring sbasic;
	tstring sfunc;
	int32 basic_sid = -1;

	std::vector<std::string> vlist = stringX::split(tmp, std::string(":"), false);
	size_t nc = vlist.size();

	if (nc != 3) fire("must and just have a basic_type and a function name");
	sname = stringX::trim(vlist[0]);
	sbasic = stringX::trim(vlist[1]);
	sfunc = stringX::trim(vlist[2]);
	
	if (sname.empty() || sbasic.empty() || sfunc.empty()) fire("sname or basic must not be empty");

	basic_sid = cg.index(sbasic);
	if (basic_sid == -1) fire("invalidate basic symbol name, must defined before use");

	int32 new_sid = cg.make_new_complex_symbol(sname, basic_sid, sfunc);
	// make new symbol
	if (new_sid == -1)
		fire(errorpos() + "invalidate complex symbol %s", sname.c_str());
	return new_sid;
}

size_t makecompiler::parse_complex_symbols(std::istream& is, compiler_grammar& cg)
{
	std::string tmp;
	while (get_nextline(is, tmp, curfile_.iLine))
	{
		if (!isspace(tmp[0])) fire(errorpos() + "failed when read complex symbol, need a whitespace");

		parse_complex_symbol(tmp, cg);
	}
	return curfile_.iLine;
}

size_t makecompiler::parse_function(std::istream& is, compiler_grammar& cg, const tstring& funcname)
{
	std::string tmp;
	while (get_nextline(is, tmp, curfile_.iLine))
	{
		if (!isspace(tmp[0])) fire(errorpos() + "failed when read function content, need a whitespace");

		compiler_grammar::funcinfo_t finfo;
		finfo.content.push_back(stringX::trim(tmp));

		if (cg.make_new_function(funcname, finfo) == -1)
			fire(errorpos() + "invalidate function %s", funcname.c_str());
	}

	return curfile_.iLine;
}


bool makecompiler::get_nextline(std::istream& is, std::string& tmp, size_t& flines) const
{
	while (is)
	{
		std::getline(is, tmp);

		if (!is) return false;
		++ flines;
		if(tmp.empty()) return false;
		else if (tmp[0] == '\r') return false;
		else if(tmp[0] == '#') continue; // comment
		else break;
	}
	return is.good();
}

void makecompiler::_replace(tstring& tmp) const
{
	std::deque<tchar> buf;
	const tchar* p = tmp.c_str();
	while (*p != '\0')
	{
		int n = _need_replace(p);
		if (n != 0)
		{
			const tstring x(p, p + n);
			const tstring& v = replacer_.find(x)->second;
			buf.insert(buf.end(), v.begin(), v.end());
			p += n;
		}
		else buf.push_back(*(p ++));
	}
	buf.push_back('\0');
	tmp.clear();
	tmp.insert(tmp.end(), buf.begin(), buf.end());
}

void makecompiler::_new_replacer(const tstring& pattern, const tstring& replacer)
{
	foreach (const strpair& p, replacer_.begin(), replacer_.end())
	{
		if (stringX::is_prefix_with(p.first, pattern) || stringX::is_prefix_with(pattern, p.first))
			fire (errorpos() + "invalidate replacer(%s), conflict with %s", pattern.c_str(), p.first.c_str());
	}

	// insert new replacer
	replacer_[pattern] == replacer;
	quick_replacer* pc = starter_.firstchild;

	const tchar* p = pattern.c_str();
	while (*p != '\0')
	{
		quick_replacer* pb = pc;
		while (pc != NULL && pc->meta != *p)
		{
			pb = pc;
			pc = pc->nextbrother;
		}
		if (pc == NULL)
		{
			// insert new meta
			while (*p)
			{
				pb->firstchild = new quick_replacer;
				pb->firstchild->meta = *p;
				pb->firstchild->firstchild = NULL;
				pb->firstchild->nextbrother = NULL;
				pb->firstchild->depth = pb->depth + 1;
				++ p;
				pb = pb->firstchild;
			}
		}
		else
		{
			++ p;
			pc = pc->firstchild;
		}
	}
}


int32 makecompiler::_need_replace(const tchar* p) const
{
	const quick_replacer* pc = starter_.firstchild;
	const quick_replacer* px = &starter_;
	if (pc == NULL) return 0;
	while (*p != '\0' && pc != NULL)
	{
		while (pc != NULL && pc->meta != *p)
			pc = pc->nextbrother;
		if (pc == NULL)
		{
			// not found
			return 0;
		}
		else
		{
			++ p;
			px = pc;
			pc = pc->firstchild;
		}
	}
	return px->depth;
}

tstring makecompiler::errorpos() const
{
	return stringX::format("[parse failed @(%s:%d)] ", curfile_.filename, curfile_.iLine);
}
