#include "unittest.h"

#include <iostream>
#include <fstream>

#include <compiler.h>
#include <extract.h>

using namespace sc;
using namespace compile;
using namespace compile::doc;

class splitwords_test : public sc::unittest
{
public:
	/* overwrite */ virtual void init(int argc, const char* argv[])
	{
		if(argc < 3)
			throw std::runtime_error("too few command arguments!\n"
					"usage: input otput");
		input = argv[1];
		otput = argv[2];
	}

	/* overwrite */ virtual void run_test()
	{
		compiler::instance().initialization();
		streamsplit wordsplit;
		std::ifstream cifs(input.c_str());
		if(!cifs.is_open()) throw std::runtime_error("can't open file: " + input);
		const streamsplit::deqwords& words = wordsplit(cifs);
		cifs.close();
		
		std::ofstream wofs(otput.c_str());
		if(!wofs.is_open()) throw std::runtime_error("can't open file: " + otput);
		for(streamsplit::deqwords::const_iterator iter = words.begin(); iter != words.end(); ++ iter)
		{
			wofs<<iter->txt<<"\t"<<iter->wordClass<<"\n";
		}
		wofs.close();
	}
private:
	std::string input;
	std::string otput;
};

NEW_UNITTEST(splitwords_test);
