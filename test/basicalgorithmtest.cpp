#include <iostream>
#include <fstream>

#include "gio.h"
#include "unittest.h"
#include <basicalgorithms.h>

using namespace compile;
using namespace compile::ga;

class basicalg_test : public sc::unittest
{
	/* overwrite */ virtual void init(int argc, const char* argv[])
	{
		if(argc < 3)
			throw std::runtime_error("too few command arguments!\n"
					"usage: input_gfile otput_file");
		fin_ = argv[1];
		fout_ = argv[2];

		// clear output file
		std::ofstream ofs(fout_.c_str());
		ofs.close();
	}

	/* overwrite */ virtual void run_test()
	{
		std::ifstream ifs(fin_.c_str());
		if(!ifs.is_open()) throw std::runtime_error("can't open file " + fin_);
		while(ifs)
		{
			greader gifs(ifs);
			grammar g;
			gifs>>g;
			test_grammar(g);
		}
		ifs.close();
	}
private:
	void test_grammar(grammar& gin)
	{
		grammar gout;
		//std::auto_ptr<grammar_algorithm> gatest(new removenotused(gin, gout));
	//	std::auto_ptr<grammar_algorithm> gatest(new eliminate_eplison(gin, gout));
		std::auto_ptr<grammar_algorithm> gatest(new removesingle(gin, gout));
		gatest->invoke();

		std::ofstream ofs(fout_.c_str(), std::ios_base::app);
		if(!ofs.is_open()) throw std::runtime_error("can't open file " + fout_);
		gwriter gofs(ofs);
		gofs<<gout;
		ofs.close();
	}

private:
	std::string fin_;
	std::string fout_;
};

NEW_UNITTEST(basicalg_test);
