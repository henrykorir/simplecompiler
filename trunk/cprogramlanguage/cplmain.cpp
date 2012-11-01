#include <fstream>
#include <iostream>
#include <typeinfo/typeinfoname.h>
#include "cplcompiler.h"

using namespace std;
using namespace kog;
using namespace compile;
using namespace compile::cplc;

//class c_compiler_main
//{
//public:
//
//};

void load_grammar_machine(tinygrammar& g, lalr1machine& m, symbol_machine& s, kog::smart_vector<cplcompiler::veccsconver>& symconvertor);

int main(int argc, const char* argv[])
{
	
	try{
		const std::string input = argv[1];
		logstring("open input file " + input);
		std::ifstream ifs(input.c_str());
		if (!ifs.is_open()) fire("can't open file "+ input);

		cplcompiler c_compiler;

		logstring("start setup compiler...");
		c_compiler.setup(load_grammar_machine);

		logstring("start init compiler...");
		c_compiler.init();


		logstring("start building...");
		c_compiler.build(ifs);

		ifs.close();
	}CATCH_EXCEPTIONS_COUT;
}

using namespace compile::doc;
extern void init_grammar(tinygrammar& tig);
extern void init_machines(std::map<std::string, machine>& machines);
extern void init_syntax_machine(lalr1machine& lrm);
extern void init_production_functions(kog::smart_vector<ifunction*>& pfuncs);
extern void init_complex_symbols(kog::smart_vector<cplcompiler::veccsconver>& convertors);

void load_grammar_machine(tinygrammar& g, lalr1machine& m, symbol_machine& s, kog::smart_vector<cplcompiler::veccsconver>& symconvertor)
{
	{
		init_grammar(g);
		logstring("init grammar done.");
	}
	{
		std::map<std::string, machine> machines;
		init_machines(machines);
		s.swap(*dynamic_cast<symbol_machine*>(machines["symbol_machine"].mac.get()));
		logstring("init symbol machine done.");
	}

	{
		kog::smart_vector<ifunction*> pfuncs;
		init_production_functions(pfuncs);
		logstring("init productions done.");

		lalr1machine tmpmachine(g, pfuncs);
		init_syntax_machine(tmpmachine);
		m.swap(tmpmachine);
		logstring("init machine done.");
	}

	{
		init_complex_symbols(symconvertor);
		logstring("init complex symbols done.");
	}
}

