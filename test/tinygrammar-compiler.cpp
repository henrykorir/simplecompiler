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
		if(argc < 3)
			throw std::runtime_error("too few command arguments!\n"
					"usage: input_xfile asmfile");
		input = argv[1];
        asmfile = argv[2];
	}

	/* overwrite */ virtual void run_test()
	{
		compiler::instance().initialization();
		compiler::instance().check(input);
        compiler::instance().generate_asm(asmfile);
	}
private:
	std::string input;
    std::string asmfile;
};

NEW_UNITTEST(tinyg_compiler_test);
