#include <iostream>
#include <fstream>
#include <memory>

#include "gio.h"
#include "unittest.h"
#include <basicalgorithms.h>

#include <lranalyse.h>
#include <scerror.h>
#include <logger.h>
#include <gensyntax.h>
#include <compilersyntax.h>
#include <compilermake.h>

using namespace compile;
using namespace compile::ga;

class syntaxgenerator_test : public sc::unittest
{
	/* overwrite */ virtual void init(int argc, const char* argv[])
	{
		if(argc < 3)
			throw std::runtime_error("too few command arguments!\n"
					"usage: input_gfile otput_file");
		fin_ = argv[1];
		cppout_ = argv[2];
	}

	/* overwrite */ virtual void run_test()
	{
		//simple_gio();
		complex_gio();
	}
private:
	void complex_gio()
	{
		fin_ = "../../test/input/cexpression.syntax";
		cppout_ = "../../test/output/cexpout.cpp";
		makecompiler mc(fin_);
		mc.build_compiler(cppout_);
	}
private:
	void simple_gio()
	{
		std::ifstream ifs(fin_.c_str());
		if(!ifs.is_open()) throw std::runtime_error("can't open file " + fin_);
		greader gifs(ifs);
		tinygrammar g;
		gifs>>g;
		ifs.close();

		//tinygrammar gouttmp;
		//eliminate_eplison ee(g, gouttmp);
		//ee.invoke();

		// remove single right production
//		removesingle rms(gouttmp, gouttmp);
//		rms.invoke();

		compiler_grammar simplegrammar;
		simplegrammar.swap_kernel(g);

		//syms[6] = Asymbol("symbol", smac, "[a-zA-Z_][a-zA-Z_0-9]*");

		simplegrammar.seperators() = "{}()=*+-/;,";
		simplegrammar.whitespaces() = " \t\r\n";
		


		grammar::vecsmacs::value_type v1("var", simplegrammar.gettinyg().index("var"), true);
		grammar::vecsmacs::value_type v2("fun", simplegrammar.gettinyg().index("fun"), true);
		simplegrammar.statemachines().resize(2);
		simplegrammar.statemachines()[0] = v1;
		simplegrammar.statemachines()[1] = v2;

		simplegrammar.keywords().resize(2);
		simplegrammar.keywords()[0] = grammar::veckeywords::value_type(v1.first, v1.second);
		simplegrammar.keywords()[1] = grammar::veckeywords::value_type(v2.first, v2.second);

		//foreach (const symbol& sym, simplegrammar.gettinyg().symbols().begin(), simplegrammar.gettinyg().symbols().end())
		//{
		//	if (!sym.ist && sym.name[])
		//}

		// output tmp gramar
		{
			logstring("[syntaxgenerator_test::run_test] tmp grammar\n");
			gwriter gw(kog::loggermanager::instance().get_logger().getos());
			gw<<simplegrammar;
		}

		syntaxgenerator gensyntax;
		gensyntax(&simplegrammar, cppout_);
	}
private:

private:
	std::string fin_;
	std::string cppout_;
};

NEW_UNITTEST(syntaxgenerator_test);
