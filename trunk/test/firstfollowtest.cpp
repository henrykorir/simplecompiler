#include <iostream>
#include <fstream>
#include <memory>

#include "gio.h"
#include "unittest.h"
#include <firstfollow.h>

using namespace compile;
using namespace compile::ga;

class firstfollow_test : public sc::unittest
{
	/* overwrite */ virtual void init(int argc, const char* argv[])
	{
		if(argc < 3)
			throw std::runtime_error("too few command arguments!"
					"usage: input_grammar output_first_follow_set");
		ginfile_ = argv[1];
		sotfile_ = argv[2];

		// clear output file
		std::ofstream ofs(sotfile_.c_str());
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
			output_first_follow_set(g);
		}
		ifs.close();
	}

	void output_first_follow_set(const tinygrammar& g)
	{
		firstset::vecintset firsts;
		firstset::vecintset follows;
		std::auto_ptr<grammar_algorithm> galg(new firstset(g, firsts));
		galg->invoke();
		std::ofstream ofs(sotfile_.c_str(), std::ios_base::app);
		if(!ofs.is_open()) throw std::runtime_error("can't open file " + sotfile_);
		
		// output first set
		outputSets(firsts, g.symbols(), ofs<<"first sets:\n", "First");
		std::auto_ptr<grammar_algorithm> g2(new followset(g, firsts, follows));
		g2->invoke();
		// output follow set
		outputSets(follows, g.symbols(), ofs<<"\nfollow sets:\n", "Follow");
		ofs.close();
	}

	void outputSets(const firstset::vecintset& sets, const symholder_proxy& sholder, std::ostream& os, const std::string& sname)
	{
		for(size_t i = 0; i < sets.size(); ++ i)
		{
			if(sholder[i].ist) continue;
			os<<sname<<"("<<sholder[i].name<<")={";
			for(firstset::vecintset::value_type::const_iterator iter = sets[i].begin(); iter != sets[i].end(); ++ iter)
			{
				os<<" "<<sholder.at(*iter).name;
			}
			os<<" }"<<std::endl;
		}
	}

	std::string ginfile_;
	std::string sotfile_;
};

NEW_UNITTEST(firstfollow_test);
