#include "gensyntax.h"
#include "compilersyntax.h"
#include "symbolmachine.h"
#include <fstream>
#include <set>

#include <statemachine.h>
#include <regex2nfa.h>
#include <nfa2dfa.h>
#include <dfa2machine.h>
#include <lrmachine.h>
#include <lranalyse.h>
#include <basicalgorithms.h>
#include <scerror.h>
#include <logger.h>
#include <combinemachines.h>
#include <typeinfo>
#include <typeinfo/typeinfoname.h>

//#define DEBUG_OUTPUT
#ifdef DEBUG_OUTPUT
#include <logger.h>
#include "../test/gio.h"
#endif

#define NEWLINE_FLUSH
#define COMBINE_REGEX_MACHINES

using namespace compile;
using namespace compile::ga;

syntaxgenerator::syntaxgenerator()
{
}

syntaxgenerator::~syntaxgenerator()
{
}

void syntaxgenerator::operator()(const grammar* aGrammar, const tstring& outfile)
{
	syntax_ = aGrammar;
	cppfile_ = outfile;
	std::ofstream ofs(cppfile_.c_str());
	if(!ofs.is_open()) throw std::runtime_error("can't open file " + cppfile_);
	try{
		cppstream_ = &ofs;
		// try to output syntax file
		print_includes();
		print_grammar();
		//print_symbols();
		print_complexsymbols();
		print_separators();
		print_keywords();
		print_statemachines();
		print_printablechars();
        print_productions();
		cppstream_ = NULL;
		ofs.close();
	} catch (sc::scerror& sce) {
		ofs.close();
		fire("inner exception is [%s] (%s)\n%s", kog::typeinfo_name(typeid(sce).name()).c_str(), sce.what(), sce.trace_message().c_str());
	}
	catch(std::exception& ex){
		ofs.close();
		fire("inner exception is [%s] (%s)\n%s", kog::typeinfo_name(typeid(ex).name()).c_str(), ex.what());
	}catch(...){
		ofs.close();
		fire("unknown exception[syntaxgenerator::operator()]!");
	}
}

std::set<tstring>& keywords()
{
	static const tchar* tmp[] =
	{
		"asm", "auto", "bad_cast", "bad_typeid", "bool", "break", "case", 
		"catch", "char", "class", "const", "const_cast", "continue", 
		"default", "delete", "do", "double", "dynamic_cast", "else", 
		"enum", "except", "explicit", "extern", "false", "finally", 
		"float", "for", "friend", "goto", "if", "inline", 
		"int", "long", "mutable", "namespace", "new", "operator", 
		"private", "protected", "public", "register", "reinterpret_cast", "return", 
		"short", "signed", "sizeof", "static", "static_cast", "struct", 
		"switch", "template", "this", "throw", "true", "try", "type_info", "typedef", 
		"typeid", "typename", "union", "unsigned", "using", "virtual", "void", "volatile", "while"
	};

	static std::set<tstring> sset;
	if (!sset.empty()) return sset;
	for (int i = 0; i < sizeof(tmp) / sizeof(const tchar*); ++ i)
	{
		sset.insert(tstring(tmp[i]));
	}
	return sset;
}

bool syntaxgenerator::is_token_keyword(const tchar* name) const
{
	const tchar* p = name;
	if (!name || !name[0]) return false;
	if (!((name[0] <= 'Z' && name[0] >= 'A') || (name[0] <= 'z' && name[0] >= 'a') || name[0] == '_')) return false;
	while (*name && ((name[0] <= 'Z' && name[0] >= 'A') || (name[0] <= 'z' && name[0] >= 'a') || name[0] == '_' || (name[0] >= '0' && name[0] <= '9')))
		++ name;
	if(*name) return false;
	return keywords().find(tstring(p)) == keywords().end();
}

void syntaxgenerator::print_symbols()
{
	const symholder_proxy& sholder = syntax_->symbols();
	std::ostream& os = *cppstream_;
	static int32 v = 0;
	tstring buf;
	for (symholder_proxy::const_iterator iter = sholder.begin(); iter != sholder.end(); ++ iter)
	{
		// system define symbol, no support now, so the name is empty
		const tchar* name = iter->name;
		if(iter->Lname == 0) continue;
		else if(!is_token_keyword(name)) name = (buf = stringX::format("NoName%d", v++)).c_str();
		os<<"\nstruct "<<name<<" : public type\n{"
				<<"\n\tenum{ sid = "<<iter->sid<<" };"
				<<"\n\t"<<name<<"()"
				<<"\n\t\t:type(sid)"
				<<"\n\t{}"
				<<"\n};\n"<<std::endl;
	}
}

struct tabident
{
private:
	static int tabs;
	int inc;
public:
	tabident(int i = 0)
		: inc(i)
	{
	}

	friend std::ostream& operator<<(std::ostream& os, const tabident& a)
	{
		tabident::tabs += a.inc;
		return os;
	}

	static std::ostream& newline(std::ostream& os)
	{
		int i = tabs;
#ifdef NEWLINE_FLUSH
		os<<std::endl;
#else
		os<<"\n";
#endif
		while(i-- > 0) os<<"\t";
		return os;
	}

	static tabident inctab;
	static tabident dectab;
};

int tabident::tabs = 0;
tabident tabident::inctab(1);
tabident tabident::dectab(-1);

tstring getstring(const tstring& str, tchar warp = '\"')
{
	const tchar* s = str.c_str();
	tstring buf(1, warp);
	while (*s)
	{
		if (*s == '\\') buf += "\\\\";
		else if(*s == '"') buf += "\\\"";
		else if(*s == '\'') buf += "\\'";
		else if(*s == '\n') buf += "\\n";
		else if(*s == '\t') buf += "\\t";
		else if(*s == '\r') buf += "\\r";
		else buf += *s;
		++ s;
	}
	return buf += warp;
}

void syntaxgenerator::print_includes()
{
	typedef std::ostream& (*pfun)(std::ostream& os);
	pfun newline = tabident::newline;
	std::ostream& os = *cppstream_;
	os<<"#include <lalr1machine.h>"
		<<newline<<"#include <compiler.h>"
		<<newline<<"#include <type.h>"
		<<newline<<"#include <treemaker.h>"
		<<newline<<"#include <deque>"
		<<newline<<"#include <lalr1machine.h>"
		<<newline<<"#include <cplcompiler.h>"
		<<newline
		<<newline<<"using namespace sc;"
		<<newline<<"using namespace compile;"
		<<newline<<"using namespace compile::doc;"
		<<newline<<"using namespace compile::runtime;"
		<<newline<<"using namespace compile::cplc;"
		<<newline
		<<newline<<"typedef lalr1machine::lalr1meta lalr1meta;"
		<<newline;
}

void syntaxgenerator::print_separators()
{
	tabident inc(tabident::inctab);
	tabident dec(tabident::dectab);
	typedef std::ostream& (*pfun)(std::ostream& os);
	pfun newline = tabident::newline;
	std::ostream& os = *cppstream_;
	// print tree
	os<<newline<<"class tree_wrap : public make_tree"
		<<newline<<"{"
		<<newline<<"public:"<<inc
		<<newline<<"void make()"
		<<newline<<"{"<<inc
		<<newline<<"link r = new_node();";
	for (tstring::const_iterator iter  = syntax_->seperators_.begin(); iter != syntax_->seperators_.end(); ++ iter)
	{
		tstring tmp(1, *iter);
		os<<newline<<"r->children["<<getstring(tmp, '\'')<<"]->v = "<<syntax_->gettinyg().index(tmp)<<";";
	}
	os<<newline<<"reset_root(r);"<<dec
		<<newline<<"}"
		<<newline<<dec
		<<newline
		<<newline<<"};";
		
	os<<newline<<"void init_separators(kog::smart_vector<sc::int32>& separators, kog::tree<int32>& sepsid)"
		<<newline<<"{"<<inc
		<<newline<<"const char tmp[] = "<<getstring(syntax_->seperators_)<<";"
		<<newline<<"separators.reset(sizeof(tmp));"
		<<newline<<"std::copy(tmp, tmp + sizeof(tmp), separators.begin());"
		<<newline<<"//// create tree"
		<<newline<<"tree_wrap* tw = reinterpret_cast<tree_wrap*>(&sepsid);"
		<<newline<<"tw->make();"
		<<newline<<dec
		<<newline<<"}"
		<<newline;
}

void syntaxgenerator::print_complexsymbols()
{
	tabident inc(tabident::inctab);
	tabident dec(tabident::dectab);
	typedef std::ostream& (*pfun)(std::ostream& os);
	pfun newline = tabident::newline;
	std::ostream& os = *cppstream_;
	size_t nsymbol_count = syntax_->symbols().size();
	os<<newline<<"void init_complex_symbols(kog::smart_vector<cplcompiler::veccsconver>& convertors)"
		<<newline<<"{"<<inc
		<<newline<<"convertors.reset("<<nsymbol_count<<");";
	if (dynamic_cast<const compiler_grammar*>(syntax_) != NULL)
	{
		const std::deque<compiler_grammar::complex_symbol_t>& csyms = dynamic_cast<const compiler_grammar*>(syntax_)->complex_symbols();
		kog::smart_vector<std::set<tstring> > usedset(nsymbol_count);
		foreach (const compiler_grammar::complex_symbol_t& s, csyms.begin(), csyms.end())
		{
			// output complex symbol
			if (s.flag != compiler_grammar::complex_symbol_t::word) continue;
			else if(usedset[s.basic_sid].find(s.content) != usedset[s.basic_sid].end()) continue;
			usedset[s.basic_sid].insert(s.content);
		}

		{
			std::set<tstring> externfuncs;
			foreach (std::set<tstring>& tmps, usedset.begin(), usedset.end())
			{
				externfuncs.insert(tmps.begin(), tmps.end());
			}
			foreach (const tstring& func, externfuncs.begin(), externfuncs.end())
			{
				os<<newline<<"extern bool "<<func<<"(automachine::machine_meta* meta);";
			}
			os<<newline;
		}

		for (size_t i = 0; i < nsymbol_count; ++ i)
		{
			if (usedset[i].empty()) continue;
			os<<newline<<"convertors["<<i<<"].resize("<<usedset[i].size()<<");";
			size_t xx = 0;
			foreach (const tstring& ctn, usedset[i].begin(), usedset[i].end())
				os<<newline<<"convertors["<<i<<"]["<<xx ++<<"] = "<<ctn<<";";
		}
	}
	os<<dec
		<<newline<<"}"
		<<newline;
}

void syntaxgenerator::print_keywords()
{
	tabident inc(tabident::inctab);
	tabident dec(tabident::dectab);
	typedef std::ostream& (*pfun)(std::ostream& os);
	pfun newline = tabident::newline;
	std::ostream& os = *cppstream_;
	os<<"void init_keywords(kog::buckethash<std::string, int32, string_2_int>& keywords)"
		<<newline<<"{"<<inc;
	const grammar::veckeywords& keywords = syntax_->keywords_;
	for (grammar::veckeywords::const_iterator iter = keywords.begin(); iter != keywords.end(); ++ iter)
	{
		os<<newline<<stringX::format("keywords.insert(%s, %d);", getstring(iter->first).c_str(), iter->second);
	}
	os<<newline<<dec
		<<newline<<"}"
		<<newline;
}

void syntaxgenerator::print_printablechars()
{
	tabident inc(tabident::inctab);
	tabident dec(tabident::dectab);
	typedef std::ostream& (*pfun)(std::ostream& os);
	pfun newline = tabident::newline;
	std::ostream& os = *cppstream_;
	os<<"void init_printablechars(kog::smart_vector<sc::int32>& printablechars)"
		<<newline<<"{"<<inc
		<<newline<<dec
		<<newline<<"}"
		<<newline;
}

void syntaxgenerator::print_statemachines()
{
	logstring("[start] syntaxgenerator::print_statemachines");
	// print some informations
	tabident inc(tabident::inctab);
	tabident dec(tabident::dectab);
	typedef std::ostream& (*pfun)(std::ostream& os);
	pfun newline = tabident::newline;
	std::ostream& os = *cppstream_;
	os<<newline<<"void init_machines(std::map<std::string, machine>& machines)"
		<<newline<<"{"<<inc;
	
	const symholder_proxy& sholder = syntax_->symbols();
	const grammar::vecsmacs& smacs = syntax_->smacs_;
#ifdef COMBINE_REGEX_MACHINES
	logstring("combine regex machines");
	if (smacs.empty()) fire("must have string machine(s)!");
	symbol_machine mout(&smacs[0], smacs.size());
	os<<newline<<newline<<"{"<<inc
		<<newline<<"kog::shared_ptr<automachine> ptr_m(new symbol_machine);"
		<<newline<<"automachine& m = *ptr_m;";
	print_machine(os, mout);
	os<<newline<<"machines[\"symbol_machine\"] = machine(ptr_m, -1);"<<dec
		<<newline<<"}";
#else
	logstring("generate regex string machines");
	for(size_t i = 0; i < smacs.size(); ++ i)
	{
		logstring("print regex string machine (%s)", smacs[i].first.c_str());
		state_machine mac;
//		state_machine::construct_fromstring(smacs[i].first, mac);
		regex_str_to_machine(smacs[i].first, mac, smacs[i].third);

		os<<newline<<newline<<"{"<<inc
			<<newline<<"kog::shared_ptr<automachine> ptr_m(new state_machine);"
			<<newline<<"automachine& m = *ptr_m;";
		print_machine(os, mac);
		os<<newline<<"machines[\""<<sholder[smacs[i].second].name<<"\"] = machine(ptr_m, "<<smacs[i].second<<");"<<dec
			<<newline<<"}";
	}
#endif
	os<<newline<<dec
		<<newline<<"}"
		<<newline;

	logstring("print lr machine");
	// print lr machine
	lrmachine lrm;
	lranalyse lra(*syntax_, lrm);
	lra.invoke();
	os<<newline<<"void init_syntax_machine(lalr1machine& lrm)"
		<<newline<<"{"<<inc
		<<newline<<"kog::shared_ptr<automachine> ptr_m(new state_machine);"
		<<newline<<"automachine& m = *ptr_m;";
	print_machine(os, lrm);
	os<<newline<<"m.swap(lrm);"
		<<newline<<dec
		<<newline<<"}"
		<<newline;
	logstring("[finish] syntaxgenerator::print_statemachines");
}

void syntaxgenerator::print_machine(std::ostream& os, const compile::automachine& mac)
{
	tabident inc(tabident::inctab);
	tabident dec(tabident::dectab);
	typedef std::ostream& (*pfun)(std::ostream& os);
	pfun newline = tabident::newline;
	const symholder_proxy& sholder = syntax_->symbols();

	// print state machines
	
	os<<newline<<"automachine::sparsesheet& sheet = m.sheet();"
		<<newline<<"sheet.reset("<<mac.sheet().size()<<");";
	// print each sheet row
	os<<newline<<"automachine::sheetrow::value_type* ptr = NULL;";
	for(size_t r = 0; r < mac.sheet().size(); ++ r)
	{
		os<<newline
			<<newline<<"sheet["<<r<<"].reset("<<mac.sheet()[r].size()<<");"
			<<newline<<"sheet["<<r<<"].type("<<mac.sheet()[r].type()<<");"
			<<newline<<"sheet["<<r<<"].endings("<<mac.sheet()[r].endings()<<");"
			<<newline<<"sheet["<<r<<"].code("<<mac.sheet()[r].code()<<");"
			<<newline<<"ptr = sheet["<<r<<"].get();"<<inc;
		for(state_machine::sheetrow::const_iterator iter = mac.sheet()[r].begin(); iter != mac.sheet()[r].end(); ++ iter)
		{
			os<<newline<<"*(ptr ++) = std::make_pair("<<iter->first<<", "<<iter->second<<");";
		}
		os<<dec;
	}
	os<<newline
		<<newline<<"m.sstate() = "<<mac.sstate()<<";";
}
	
void syntaxgenerator::regex_str_to_machine(const std::string& regexstr, automachine& m, bool is_directly)
{
	if(is_directly) logstring("convert string(%s) to automachine", regexstr.c_str());
	else logstring("convert regex string(%s) to automachine", regexstr.c_str());
	grammar nfa, dfa;
	ga::regex2nfa r2n(regexstr, nfa, is_directly);
	ga::nfa2dfa n2d(nfa, dfa);
	ga::dfa2machine d2m(dfa, m);

#ifdef DEBUG_OUTPUT
	logstring("[syntaxgenerator::regex_str_to_machine]");
	std::ostream& ofslog = kog::loggermanager::instance().get_logger().getos();
	gwriter gofs(ofslog);
	r2n.invoke();
	ofslog<<"nfa result:\n";
	gofs<<nfa;
	ofslog<<std::endl;
	n2d.invoke();
	ofslog<<"dfa result:\n";
	gofs<<dfa;
	ofslog<<std::endl;
	d2m.invoke();
#else
	r2n.invoke();
	n2d.invoke();
	d2m.invoke();
#endif

	// because we remove the eplison in nfa2dfa stage
	//	so, every charactor's sid hava become small, we must let it correct
	refine_machine(m);
}

void syntaxgenerator::refine_machine(automachine& m) const
{
	automachine::sparsesheet& sheet = m.sheet();
	for (size_t i = 0; i < sheet.size(); ++ i)
	{
		automachine::sheetrow& row = sheet[i];
		for(state_machine::sheetrow::iterator iter = row.begin(); iter != row.end(); ++ iter)
		{
			++ iter->first;
		}
	}
}

void syntaxgenerator::print_grammar()
{
	tabident inc(tabident::inctab);
	tabident dec(tabident::dectab);
	typedef std::ostream& (*pfun)(std::ostream& os);
	std::ostream& os = *cppstream_;
	pfun newline = tabident::newline;
	const symholder_proxy& sholder = syntax_->symbols();
	const prodholder_proxy& pholder = syntax_->productions();

	const tinygrammar& tig = syntax_->gettinyg();

	os<<newline<<"void init_grammar(tinygrammar& tig)"
		<<newline<<"{"<<inc
		<<newline<<"//// create symbols list: "
		<<newline<<"std::deque<symbol> slist;"
		<<newline<<"slist.resize("<<sholder.size()<<");"
		<<newline<<"{"<<inc;
	for (size_t i = 0; i < sholder.size(); ++ i)
	{
		tstring x = stringX::format("slist[%d]", i);
		os<<newline<<x<<".sid = "<<sholder[i].sid<<";"
			<<newline<<x<<".funcs = NULL;"
			<<newline<<x<<".Lname = "<<sholder[i].Lname<<";"
			<<newline<<x<<".Lfuncs = 0;"
			<<newline<<x<<".ist = "<<(sholder[i].ist ? "1;" : "0;")
			<<newline<<x<<".name = "<<getstring(sholder[i].name)<<";"
			<<newline;
	}
	os<<dec<<newline<<"}"
		<<newline<<"//// create productions"
		<<newline<<"prodholder_proxy plist;"
		<<newline<<"plist.reset("<<pholder.size()<<");"
		<<newline<<"{"<<inc;
	for (size_t i = 0; i < pholder.size(); ++ i)
	{
		const production& p = pholder[i];
		tstring y = stringX::format("tmpv_%d", i);
		os<<newline<<"int32 "<<y<<"[] = {";
		const tchar* sep = "";
		for (production::right_array::const_iterator iter = p.right().begin(); iter != p.right().end(); ++ iter, sep = ", ")
		{
			os<<sep<<*iter;
		}
		os<<"};"
			<<newline<<"plist["<<i<<"] = production("<<p.left()<<", "<<y<<", sizeof("<<y<<")/sizeof(int32));"
			<<newline;
	}
	os<<dec<<newline<<"}"
		<<newline
		<<newline<<"tinygrammar tmptinyg(slist.begin(), slist.end(),"<<inc
		<<newline<<"plist.begin(), plist.end(), "
		<<newline<<syntax_->starts()<<", "
			<<syntax_->eplisons()<<", "
			<<syntax_->endings()<<");"<<dec
		<<newline<<"tmptinyg.swap(tig);"
		<<newline<<dec
		<<newline<<"}"
		<<newline;
}

void syntaxgenerator::print_productions()
{
	bool using_gs = dynamic_cast<const compiler_grammar*>(syntax_) != NULL;
	const prodholder_proxy& pholder = syntax_->productions();
    function_parser fp(*cppstream_);
    for (size_t i = 0; i < pholder.size(); ++ i)
    {
		if (using_gs)
		{
			const compiler_grammar* cg = dynamic_cast<const compiler_grammar*>(syntax_);
			//const compiler_grammar::prodmoreinfos&
			fp(pholder[i].to_string(), cg->prodmoreinfos()[i], stringX::format("production_func_%d", i));
		}
		else
		{
			fp(pholder[i].to_string(), pholder[i].func(), stringX::format("production_func_%d", i));
		}
    }

	// print init_production function
	std::ostream& os = *cppstream_;
	tabident inc(tabident::inctab);
	tabident dec(tabident::dectab);
	typedef std::ostream& (*pfun)(std::ostream& os);
	pfun newline = tabident::newline;
	os<<newline<<newline<<"void init_production_functions(kog::smart_vector<ifunction*>& pfuncs)"
		<<newline<<"{"<<inc
		<<newline<<"pfuncs.reset("<<pholder.size()<<");";
	for (size_t i = 0; i < pholder.size(); ++ i)
	{
		os<<newline<<"pfuncs["<<i<<"] = new production_func_"<<i<<"();";
	}
	os<<dec<<newline<<"}";
}

#define CONTENT_BEGIN "{"
#define CONTENT_END "}"
#define ENTRYV "entry_v"
#define ENTRYF "entry_f"
#define CALLF "call"
#define TRIPLE "triple"
#define SEP ";"

function_parser& function_parser::operator()(const _Str& comment, const _Str& func, const _Str& name)
{
	typedef std::ostream& (*pfun)(std::ostream& os);
    std::ostream& os = *cppstream_;
	tabident inc(tabident::inctab);
	tabident dec(tabident::dectab);
	pfun newline = tabident::newline;
    os<<newline<<"//"<<comment
		<<newline<<"struct "<<name<<" : public ifunction"
        <<newline<<"{"<<inc
        <<newline<<"/* overwrite */ virtual machine_meta* operator()(machine_meta*const* metas, int C, machine_meta* result)"
        <<newline<<"{"<<inc
		//<<newline<<"const lalr1meta* m0 = (const lalr1meta*)(metas[0]);"
		//<<newline<<"lalr1meta* ot = (lalr1meta*)(result);"
		//<<newline<<"ot->content = m0->content;"
		//<<newline<<"ot->ctype = m0->ctype;"
		//<<newline<<"return result;"
        //<<newline<<"result->content = metas[0]->content;"
        //<<newline<<"result->v = metas[0]->v;";
       ;
    

    size_t posb = func.find_first_of(CONTENT_BEGIN);
    size_t pose = func.find_last_of(CONTENT_END);

    if (posb == _Str::npos || pose == _Str::npos)
    {
        //fire("invalidate function, " + func); 
    }
    else
    {
        stringX::string_split_t<_Str::value_type> ss(func.substr(posb + 1, pose - posb - 2), ";", true);
        for (stringX::string_split_t<_Str::value_type>::const_iterator iter = ss.begin(); iter != ss.end(); ++ iter)
        {
            os<<newline<<*iter<<";";
        }
    }

    os<<dec<<newline<<"}"<<dec
        <<newline<<"};\n";
        
    return *this;
}

function_parser& function_parser::operator()(const _Str& comment, const compiler_grammar::prodinfo_t& infos, const _Str& name)
{
	typedef std::ostream& (*pfun)(std::ostream& os);
    std::ostream& os = *cppstream_;
	tabident inc(tabident::inctab);
	tabident dec(tabident::dectab);
	pfun newline = tabident::newline;
    os<<newline<<"//"<<comment
		<<newline<<"struct "<<name<<" : public ifunction"
        <<newline<<"{"<<inc
        <<newline<<"/* overwrite */ virtual machine_meta* operator()(machine_meta*const* metas, int C, machine_meta* result)"
        <<newline<<"{"<<inc;

	os<<newline<<"return result;";
	foreach(const tstring& pinfo, infos.content.begin(), infos.content.end())
	{
		//os<<newline<<pinfo<<";";
	}

    os<<dec<<newline<<"}"<<dec
        <<newline<<"};\n";
        
    return *this;
}
