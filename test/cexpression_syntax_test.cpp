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

using namespace sc;
using namespace compile;
using namespace compile::ga;

class cexpression_test : public unittest
{
public:
	/* overwrite */ virtual void init(int argc, const char* argv[])
	{
		ASSERT_COMMAND_ARGUMENTS(2, "usage: input_xfile");

		input = argv[1];
        //asmfile = argv[2];
	}

	/* overwrite */ virtual void run_test()
	{
		compiler::instance().initialization();
		compiler::instance().check(input);
        //compiler::instance().generate_asm(asmfile);
	}
private:
	std::string input;
    //std::string asmfile;
};

NEW_UNITTEST(cexpression_test);
