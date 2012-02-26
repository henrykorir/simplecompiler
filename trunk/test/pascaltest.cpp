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

using namespace compile;
using namespace compile::ga;

class pascal_test : public sc::unittest
{
	/* overwrite */ virtual void init(int argc, const char* argv[])
	{
		ASSERT_COMMAND_ARGUMENTS(1, "output_folder");
		ofolder_ = argv[1];
	}

	/* overwrite */ virtual void run_test()
	{
		grammar pascal;
		load_grammar(pascal);

		lranalyse lra(pascal, m);
		lra.invoke();

		syntaxgenerator gensyntax;
		const std::string cppfile = ofolder_ + "/pascal_type.cpp"
		gensyntax(&pascal, cppfile);
	}

	void output_machine(const lrmachine& m)
	{
		const std::string fname = ofolder_ + "/pascal_lrmachine.txt";
		std::ofstream ofs(fname.c_str());
		if(!ofs.is_open()) fire("can't open file " + fname);

		lrmwriter lrmw(ofs);
		lrmw<<m;

		ofs.close();
	}

	void load_grammar(grammar& gin)
	{
		const std::string fname = "test/input/pascal.txt";
		std::ifstream ifs(fname.c_str());
		if(!ifs.is_open()) throw std::runtime_error("can't open file " + fname);
		greader gifs(ifs);
		gifs>>gin;
		ifs.close();
	}

	std::string ofolder_;
};

NEW_UNITTEST(pascal_test);