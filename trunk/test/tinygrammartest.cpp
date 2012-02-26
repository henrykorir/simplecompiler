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

using namespace sc;
using namespace compile;
using namespace compile::ga;

class simplegrammar_test : public unittest
{
public:
	/* overwrite */ virtual void init(int argc, const char* argv[])
	{
		ASSERT_COMMAND_ARGUMENTS(3, "output_cppfile output_macfile");
		cppfile = argv[1];
		macfile = argv[2];
	}

	/* overwrite */ virtual void run_test()
	{
		grammar simplegrammar;
		init_grammar(simplegrammar);
		
		syntaxgenerator gensyntax;
		gensyntax(&simplegrammar, cppfile);

		// remove eplison first
		eliminate_eplison ee(simplegrammar, simplegrammar);
		ee.invoke();

		// remove single right production
//		removesingle rms(simplegrammar, simplegrammar);
//		rms.invoke();

		// output tmp gramar
		{
			logstring("[simplegrammar_test::run_test] tmp grammar\n");
			gwriter gw(kog::loggermanager::instance().get_logger().getos());
			gw<<simplegrammar;
		}

		lrmachine m;
		lranalyse lra(simplegrammar, m);
		lra.invoke();

		// output lr machine
		{
			std::ofstream ofs(macfile.c_str());
			if(!ofs.is_open()) fire("can't open file " + macfile);

			lrmwriter lrmw(ofs);
			lrmw<<m;

			ofs.close();
		}
	}
private:
	void init_grammar(grammar& g);
private:
	std::string cppfile;
	std::string macfile;
};

class grammar_wrapper : public grammar
{
public:
	void simple_grammar();
};

void simplegrammar_test::init_grammar(grammar& g)
{
	grammar_wrapper* p = reinterpret_cast<grammar_wrapper*>(&g);
	p->simple_grammar();
}

/* tiny grammar
 *
 * Program -> AProgramItem | AProgramItem Program
 * AProgramItem -> ValueDeclear | Function | Expression
 * ValueDeclear -> type symbol;
 * Expression -> AExpression Expression | AExpression
 * AExpression -> symbol = AValue; | symbol = AValue op AValue; | FuncCall; | Print;
 * AValue -> symbol | number;
 * FuncCall -> symbol = symbol(AValue, AValue)
 * Print -> print(AValue)
 * Function -> type symbol(type symbol, type symbol) { FunctionContent ReturnExp }
 * FunctionContent -> Expression FunctionContent | ValueDeclear FunctionContent | Empty
 * ReturnExp -> return AValue;
 *
 * whitespace: " \t\r\n"
 * Seperators: "{}()=*+-/;,"
 * KeyWords: int float print return
 * type: int, float
 * op: + - * /
 * number: ([0-9]+)|([0-9]+\.[0-9]*)|(\.[0-9]+)
 * symbol: [a-zA-Z_][a-zA-Z_-0-9]*
*/
void grammar_wrapper::simple_grammar()
{
	symholder& syms = symbols_;
	vecprods& prods = productions_;

	whitespaces_ = " \t\r\n";
	start_symbol_ = 0; // start symbol
	dthenu_ = true;

	seperators_ = "{}()=*+-/;,";
	syms.reset(20 + seperators_.size());
	syms[0] = Asymbol("Program", 0);
	syms[1] = Asymbol("AProgramItem", 0);
	syms[2] = Asymbol("ValueDeclear", 0);
	syms[3] = Asymbol("Function", 0);
	syms[4] = Asymbol("Expression", 0);
	syms[5] = Asymbol("valuetype", 0);
	syms[6] = Asymbol("symbol", smac, "[a-zA-Z_][a-zA-Z_0-9]*");
	syms[7] = Asymbol("AExpression", 0);
	syms[8] = Asymbol("AValue", 0);
	syms[9] = Asymbol("number", smac, "([0-9]+)|([0-9]+\\.[0-9]*)|(\\.[0-9]+)");
	syms[10] = Asymbol("FuncCall", 0);
	syms[11] = Asymbol("op", 0);
	syms[12] = Asymbol("Print", 0);
	syms[13] = Asymbol("printF", keyword, "print");
	syms[14] = Asymbol("FunctionContent", 0);
	syms[15] = Asymbol("ReturnExp", 0);
	syms[16] = Asymbol("Return", keyword, "return");
	syms[17] = Asymbol("Int", keyword, "int");
	syms[18] = Asymbol("Float", keyword, "float");
	syms[19] = Asymbol("", other_sym, "eplison");
	for(size_t i = 0; i < seperators_.size(); ++ i)
		//syms[i + 20] = Asymbol(stringX::format("seperator%d", i), sep, tstring(1, seperators_[i]));
		syms[i + 20] = Asymbol(std::string(1, seperators_[i]), sep, tstring(1, seperators_[i]));
	eplisons() = 19;
	// reset symbols name string
	{
		symholder newholder(Asymbol::stringBuf.begin(), Asymbol::stringBuf.end(), 
				syms.begin(), syms.end());
		for(symholder::iterator iter = newholder.begin(); iter != newholder.end(); ++ iter)
		{
			iter->name = newholder.getstringptr() + (int32)(iter->name);
		}
		newholder.swap(syms);
		syms.make_index();
	}

	// set keywords and statemachines
	{
		keywords_.reset(Asymbol::keywords.size());
		smacs_.reset(Asymbol::smacs.size());
		std::copy(Asymbol::smacs.begin(), Asymbol::smacs.end(), smacs_.begin());
	}
	
	prods.reset(27);
	V::symbols() = &syms;
	prods[0] = Aproduction(0, V(1));
	prods[1] = Aproduction(0, V(1, 0));
	prods[2] = Aproduction(1, V(2));
	prods[3] = Aproduction(1, V(3));
	prods[4] = Aproduction(1, V(4));
	prods[5] = Aproduction(2, V(5, 6));
	prods[6] = Aproduction(4, V(7, 4));
	prods[7] = Aproduction(4, V(7));
	prods[8] = Aproduction(7, V(6, "=", 8, ";"));
	prods[9] = Aproduction(7, V(6, "=", 8, 11, 8, ";"));
	prods[10] = Aproduction(7, V(10, ";"));
	prods[11] = Aproduction(7, V(12, ";"));
	prods[12] = Aproduction(8, V(6));
	prods[13] = Aproduction(8, V(9));
	prods[14] = Aproduction(10, V(6, "=", "(", 6, ",", 6, ")"));
	prods[15] = Aproduction(12, V(13, "(", 8, ")"));
	prods[16] = Aproduction(3, V(5, 6, "(", 5, 6, ",", 5, 6, ")", "{", 14, 15, "}"));
	prods[17] = Aproduction(14, V(4, 14));
	prods[18] = Aproduction(14, V(2, 14));
	prods[19] = Aproduction(14, V(19));
	prods[20] = Aproduction(15, V(16, 8, ";"));
	prods[21] = Aproduction(5, V(17));
	prods[22] = Aproduction(5, V(18));
	prods[23] = Aproduction(11, V("+"));
	prods[24] = Aproduction(11, V("-"));
	prods[25] = Aproduction(11, V("*"));
	prods[26] = Aproduction(11, V("/"));
}

std::deque<tchar> Asymbol::stringBuf;
std::deque<std::pair<tstring, int32> > Asymbol::keywords;
std::deque<std::pair<tstring, int32> > Asymbol::smacs;

NEW_UNITTEST(simplegrammar_test);
