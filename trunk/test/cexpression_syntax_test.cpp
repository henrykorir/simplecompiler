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
#include <lalr1machine.h>
#include <CExpTest_compiler.h>

using namespace sc;
using namespace compile;
using namespace compile::ga;
using namespace compile::runtime;

//void load_grammar_machine(tinygrammar& g, lalr1machine& m, symbol_machine& s, kog::smart_vector<cplcompiler::veccsconver>& symconvertor);
//extern void print_nodes();
class cexpression_test : public unittest
{
public:
	/* overwrite */ virtual void init(int argc, const char* argv[])
	{
		ASSERT_COMMAND_ARGUMENTS(2, "usage: input_xfile");

		input = argv[1];
	}

	/* overwrite */ virtual void run_test()
	{
		logstring("open input file " + input);

		compiler_setup* c_expression_compiler = new CExpTest;

		logstring("start setup compiler...");
		compiler::instance().setup(c_expression_compiler);

		logstring("start init compiler...");
		compiler::instance().init(input);


		logstring("start building...");
		compiler::instance().build_src2iml();
	}
private:
	std::string input;
};

NEW_UNITTEST(cexpression_test);

//void entry_types()
//{
//	typesystem::instance().new_type<type>("struct_type")->tsize = 4; // struct_type
//	typesystem::instance().new_type<type>("enum_type")->tsize = 4; // enum_type
//	typesystem::instance().new_type<type>("union_type")->tsize = 4; // enum_type
//	typesystem::instance().new_type<type>("void")->tsize = 0; // enum_type
//    typesystem::instance().new_type<type>("int")->tsize = sizeof(int32); // int_type
//    typesystem::instance().new_type<type>("float")->tsize = sizeof(float); // float_type
//    typesystem::instance().new_type<type>("double")->tsize = sizeof(double); // double_type
//    typesystem::instance().new_type<type>("short")->tsize = sizeof(short); // short_type
//    typesystem::instance().new_type<type>("char")->tsize = sizeof(char); // char_type
//    typesystem::instance().new_type<type>("long")->tsize = sizeof(long); // long_type
//    typesystem::instance().new_type<type>("long long")->tsize = sizeof(long long); // long_long_type
//    typesystem::instance().new_type<type>("unsigned char")->tsize = sizeof(unsigned char); // uchar_type
//    typesystem::instance().new_type<type>("unsigned short")->tsize = sizeof(unsigned short); // ushort_type
//    typesystem::instance().new_type<type>("unsigned int")->tsize = sizeof(unsigned int); // uint_type
//    typesystem::instance().new_type<type>("unsigned long")->tsize = sizeof(unsigned long); // ulong_type
//    typesystem::instance().new_type<type>("unsigned long long")->tsize = sizeof(unsigned long long); // ulong_long_type
//}
//
//using namespace compile::doc;
//
//void load_grammar_machine(tinygrammar& g, lalr1machine& m, symbol_machine& s, kog::smart_vector<cplcompiler::veccsconver>& symconvertor)
//{
//	{
//		init_grammar(g);
//		logstring("init grammar done.");
//	}
//	{
//		std::map<std::string, machine> machines;
//		init_machines(machines);
//		s.swap(*dynamic_cast<symbol_machine*>(machines["symbol_machine"].mac.get()));
//		logstring("init symbol machine done.");
//	}
//
//	{
//		kog::smart_vector<ifunction*> pfuncs;
//		init_production_functions(pfuncs);
//		logstring("init productions done.");
//
//		lalr1machine tmpmachine(g, pfuncs);
//		init_syntax_machine(tmpmachine);
//		m.swap(tmpmachine);
//		logstring("init machine done.");
//	}
//
//	{
//		init_complex_symbols(symconvertor);
//		logstring("init complex symbols done.");
//	}
//}

//void init_grammar(tinygrammar& tig) {}
//void init_machines(std::map<std::string, machine>& machines) {}
//void init_syntax_machine(lalr1machine& lrm) {}
//void init_keywords(kog::buckethash<std::string, int32, string_2_int>& keywords) {}
//void init_separators(kog::smart_vector<sc::int32>& separators, kog::tree<int32>& sepsid) {}
//void init_printablechars(kog::smart_vector<sc::int32>& printablechars) {}
//void init_production_functions(kog::smart_vector<ifunction*>& pfuncs) {}
