// 
#include <iostream>
#include <fstream>

#include <stringX.h>

#include "unittest.h"
#include "grammar.h"
#include "gensyntax.h"
#include "vhelp.h"
#include "lrmio.h"
#include "gio.h"

#include <lranalyse.h>
#include <basicalgorithms.h>
#include <scerror.h>
#include <logger.h>
#include <compiler.h>
#include <extract.h>
#include <cplcompiler.h>
#include <lalr1machine.h>
#include <cplcompiler.h>

using namespace sc;
using namespace compile;
using namespace compile::ga;
using namespace compile::cplc;

void load_grammar_machine(tinygrammar& g, lalr1machine& m, symbol_machine& s, kog::smart_vector<cplcompiler::veccsconver>& symconvertor);
class cexpression_test : public unittest
{
public:
	/* overwrite */ virtual void init(int argc, const char* argv[])
	{
		ASSERT_COMMAND_ARGUMENTS(2, "usage: input_xfile");

		input = argv[1];
	}

	/* overwrite */ virtual void run_test()
	{
		logstring("open input file " + input);
		std::ifstream ifs(input.c_str());
		if (!ifs.is_open()) fire("can't open file "+ input);

		cplcompiler c_expression_compiler;

		logstring("start setup compiler...");
		c_expression_compiler.setup(load_grammar_machine);

		logstring("start init compiler...");
		c_expression_compiler.init();


		logstring("start building...");
		c_expression_compiler.build(ifs);

		ifs.close();
	}
private:
	std::string input;
};

NEW_UNITTEST(cexpression_test);

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

//void init_grammar(tinygrammar& tig) {}
//void init_machines(std::map<std::string, machine>& machines) {}
//void init_syntax_machine(lalr1machine& lrm) {}
//void init_keywords(kog::buckethash<std::string, int32, string_2_int>& keywords) {}
//void init_separators(kog::smart_vector<sc::int32>& separators, kog::tree<int32>& sepsid) {}
//void init_printablechars(kog::smart_vector<sc::int32>& printablechars) {}
//void init_production_functions(kog::smart_vector<ifunction*>& pfuncs) {}
