#include "unittest.h"

#include <iostream>
#include <fstream>

#include <compiler.h>
#include <extract.h>

using namespace sc;
using namespace compile;

class tinyg_compiler_test : public sc::unittest
{
public:
	/* overwrite */ virtual void init(int argc, const char* argv[])
	{
		if(argc < 2)
			throw std::runtime_error("too few command arguments!\n"
					"usage: input_xfile");
		input = argv[1];
	}

	/* overwrite */ virtual void run_test()
	{
		compiler::instance().initialization();
		compiler::instance().check(input);
	}
private:
	std::string input;
};

NEW_UNITTEST(tinyg_compiler_test);