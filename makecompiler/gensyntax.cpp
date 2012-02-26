#include "gensyntax.h"
#include <fstream>

#include <statemachine.h>
#include <regex2nfa.h>
#include <nfa2dfa.h>
#include <dfa2machine.h>

//#define DEBUG_OUTPUT
#ifdef DEBUG_OUTPUT
#include <iostream>
#include "../test/gio.h"
#endif

using namespace compile;

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
		print_symbols();
		print_statemachines();
		cppstream_ = NULL;
		ofs.close();
	}catch(std::exception& ex){
		ofs.close();
		throw ex;
	}catch(...){
		ofs.close();
		throw std::runtime_error("unknown exception[syntaxgenerator::operator()]!");
	}
}

void syntaxgenerator::print_symbols()
{
	const symholder& sholder = syntax_->symbols();
	std::ostream& os = *cppstream_;
	for (symholder::const_iterator iter = sholder.begin(); iter != sholder.end(); ++ iter)
	{
		// system define symbol, no support now, so the name is empty
		if(iter->Lname == 0) continue;
		os<<"\nstruct "<<iter->name<<" : public type\n{"
				<<"\n\tenum{ sid = "<<iter->sid<<" };"
				<<"\n\t"<<iter->name<<"()"
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
		os<<"\n";
		while(i-- > 0) os<<"\t";
		return os;
	}

	static tabident inctab;
	static tabident dectab;
};

int tabident::tabs = 0;
tabident tabident::inctab(1);
tabident tabident::dectab(-1);


void syntaxgenerator::print_statemachines()
{
	// print some informations
	tabident inc(tabident::inctab);
	tabident dec(tabident::dectab);
	typedef std::ostream& (*pfun)(std::ostream& os);
	pfun newline = tabident::newline;
	std::ostream& os = *cppstream_;
	os<<newline<<"void initmachines(compiler& c)"
		<<newline<<"{"<<inc;
	
	const symholder& sholder = syntax_->symbols();
	const grammar::vecsmacs& smacs = syntax_->smacs_;
	for(size_t i = 0; i < smacs.size(); ++ i)
	{
		state_machine mac;
//		state_machine::construct_fromstring(smacs[i].first, mac);
		regex_str_to_machine(smacs[i].first, mac);
		// print state machines
		os<<newline
			<<newline<<"{"<<inc
			<<newline<<"state_machine m;"
			<<newline<<"state_machine::sparsesheet& sheet = m.sheet();"
			<<newline<<"sheet.reset("<<mac.sheet().size()<<");";
		// print each sheet row
		os<<newline<<"state_machine::sheetrow::value_type* ptr = NULL;";
		for(size_t r = 0; r < mac.sheet().size(); ++ r)
		{
			os<<newline
				<<newline<<"sheet["<<r<<"].reset("<<mac.sheet()[r].size()<<");"
				<<newline<<"sheet["<<r<<"].type("<<mac.sheet()[r].type()<<");"
				<<newline<<"sheet["<<r<<"].endings("<<mac.sheet()[r].endings()<<");"
				<<newline<<"ptr = sheet["<<r<<"].get()"<<inc;
			for(state_machine::sheetrow::iterator iter = mac.sheet()[r].begin(); iter != mac.sheet()[r].end(); ++ iter)
			{
				os<<newline<<"*(ptr ++) = std::make_pair("<<iter->first<<", "<<iter->second<<");";
			}
			os<<dec;
		}
		os<<newline
			<<newline<<"m.sstate() = "<<mac.sstate()<<";"
			<<newline<<"c[\""<<sholder[smacs[i].second].name<<"\"] = m;"<<dec
			<<newline<<"}";
	}
	os<<newline<<dec
		<<newline<<"}"
		<<newline;
}
	
void syntaxgenerator::regex_str_to_machine(const std::string& regexstr, automachine& m)
{
	grammar nfa, dfa;
	ga::regex2nfa r2n(regexstr, nfa);
	ga::nfa2dfa n2d(nfa, dfa);
	ga::dfa2machine d2m(dfa, m);

#ifdef DEBUG_OUTPUT
	static std::ofstream ofslog("runlog.txt");
	gwriter gofs(ofslog);
	r2n.invoke();
	gofs<<nfa;
	ofslog<<std::endl;
	n2d.invoke();
	gofs<<dfa;
	ofslog<<std::endl;
	d2m.invoke();
//	ofslog.close();
#else
	r2n.invoke();
	n2d.invoke();
	d2m.invoke();
#endif
}