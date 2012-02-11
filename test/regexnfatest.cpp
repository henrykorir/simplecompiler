#include <iostream>
#include <fstream>

#include "gio.h"
#include "unittest.h"
#include <regex2nfa.h>

using namespace compile;
using namespace compile::ga;

class regex2nfa_test : public sc::unittest
{
	/* overwrite */ virtual void init(int argc, const char* argv[])
	{
		outfile = "test/output/regex2nfa.txt";
		// clear output file
		std::ofstream ofs(outfile.c_str());
		ofs.close();
	}

	/* overwrite */ virtual void run_test()
	{
		const tchar* strs[] = 
		{
	//		"abcfd",
	//		"ac*d+|a",
	//		"x((y))z"
	//		"x(a|b)z"
	//		"a(v)+"
	//		"a(b((df|a)c(a*)(b+))+)c"
	//		"a[abc]d"
	//		"(a*(([xyz]d|bc))(v|q)|tmp+)"
			"[a-zA-Z_][a-zA-Z0-9_]+",
			"([0-9]+)|([0-9]+\\.[0-9]*)|(\\.[0-9]+)"
		};

		for(int i = 0; i < sizeof(strs) / sizeof(const tchar*); ++ i)
		{
			grammar otputg;
			regex2nfa r2n(strs[i], otputg);
			r2n.invoke();
			print_grammar_to_file(otputg);
		}
	}

	void print_grammar_to_file(const grammar& g)
	{
		std::ofstream ofs(outfile.c_str(), std::ios_base::app);
		if(!ofs.is_open()) throw std::runtime_error("can't open file " + outfile);
		gwriter gofs(ofs);
		gofs<<g;
		ofs<<std::endl;
		ofs.close();
	}

	std::string outfile;
};

NEW_UNITTEST(regex2nfa_test);
