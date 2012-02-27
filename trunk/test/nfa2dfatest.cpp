#include <iostream>
#include <fstream>
#include <memory>

#include "gio.h"
#include "unittest.h"
#include <nfa2dfa.h>

using namespace compile;
using namespace compile::ga;

class nfa2dfa_test : public sc::unittest
{
	/* overwrite */ virtual void init(int argc, const char* argv[])
	{
		if(argc < 3)
			throw std::runtime_error("too few comand arguments!"
					"usage: input_grammar_file output_grammar_file");
		ginfile_ = argv[1];
		gotfile_ = argv[2];
		
		// clear output file
		std::ofstream ofs(gotfile_.c_str());
		ofs.close();
	}

	/* overwrite */ virtual void run_test()
	{
		std::ifstream ifs(ginfile_.c_str());
		if(!ifs.is_open()) throw std::runtime_error("can't open file " + ginfile_);
		greader gifs(ifs);
		while(ifs)
		{
			tinygrammar g;
			gifs>>g;
			todfa(g);
		}
		ifs.close();
	}

	void todfa(const tinygrammar& gin)
	{
		tinygrammar got;
		std::auto_ptr<grammar_algorithm> galg(new nfa2dfa(gin, got));
		galg->invoke();
		std::ofstream ofs(gotfile_.c_str(), std::ios_base::app);
		if(!ofs.is_open()) throw std::runtime_error("can't open file " + gotfile_);
		gwriter gofs(ofs);
		gofs<<got;
		ofs.close();
	}

	std::string ginfile_;
	std::string gotfile_;
};

NEW_UNITTEST(nfa2dfa_test);
