#include <fstream>
#include <iostream>
#include <typeinfo/typeinfoname.h>
#include "cplcompiler.h"

using namespace std;
using namespace kog;
using namespace compile;
using namespace compile::cplc;

//class c_compiler_main
//{
//public:
//
//};

void load_grammar_machine(tinygrammar& g, lalr1machine& m, symbol_machine& s, kog::smart_vector<cplcompiler::veccsconver>& symconvertor);

int main(int argc, const char* argv[])
{
	
	try{
		const std::string input = argv[1];
		logstring("open input file " + input);
		std::ifstream ifs(input.c_str());
		if (!ifs.is_open()) fire("can't open file "+ input);

		cplcompiler c_compiler;

		logstring("start setup compiler...");
		c_compiler.setup();

		logstring("start init compiler...");
		c_compiler.init();


		logstring("start building...");
		c_compiler.build(ifs);

		ifs.close();
	}CATCH_EXCEPTIONS_COUT;
}
