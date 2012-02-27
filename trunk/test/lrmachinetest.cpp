#include <iostream>
#include <fstream>
#include <memory>

#include "gio.h"
#include "lrmio.h"
#include "unittest.h"
#include <lranalyse.h>
#include <scerror.h>

using namespace compile;
using namespace compile::ga;

class lrmachine_test : public sc::unittest
{
	/* overwrite */ virtual void init(int argc, const char* argv[])
	{
		ASSERT_COMMAND_ARGUMENTS(3, "input_grammar output_machine");
		gfile_ = argv[1];
		mfile_ = argv[2];
	}

	/* overwrite */ virtual void run_test()
	{
		tinygrammar g;
		std::ifstream ifs(gfile_.c_str());
		if(!ifs.is_open()) throw std::runtime_error("can't open file " + gfile_);
		greader gifs(ifs);
		while(ifs)
		{
			tinygrammar g;
			lrmachine m;
			gifs>>g;
			lranalyse lra(g, m);
			lra.invoke();

			output_machine(m);
		}
		ifs.close();
	}

	void output_machine(const lrmachine& m)
	{
		std::ofstream ofs(mfile_.c_str());
		if(!ofs.is_open()) fire("can't open file " + mfile_);

		lrmwriter lrmw(ofs);
		lrmw<<m;

		ofs.close();
	}

	std::string gfile_;
	std::string mfile_;
};

NEW_UNITTEST(lrmachine_test);