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

class combinemachines_test : public unittest
{
public:
	/* overwrite */ virtual void init(int argc, const char* argv[])
	{
	}

	/* overwrite */ virtual void run_test()
	{
	}
};

NEW_UNITTEST(combinemachines_test);