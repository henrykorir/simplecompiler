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
		ASSERT_COMMAND_ARGUMENTS(2, "output_cppfile");
		cppfile = argv[1];
	}

	/* overwrite */ virtual void run_test()
	{
		grammar simplegrammar;
		init_grammar(simplegrammar);

		// remove eplison first
		tinygrammar gouttmp;
		eliminate_eplison ee(simplegrammar.gettinyg(), gouttmp);
		ee.invoke();

		// remove single right production
//		removesingle rms(gouttmp, gouttmp);
//		rms.invoke();

		simplegrammar.swap_kernel(gouttmp);

		// output tmp gramar
		{
			logstring("[simplegrammar_test::run_test] tmp grammar\n");
			gwriter gw(kog::loggermanager::instance().get_logger().getos());
			gw<<simplegrammar;
		}

		syntaxgenerator gensyntax;
		gensyntax(&simplegrammar, cppfile);
	}
private:
	void init_grammar(grammar& g);
private:
	std::string cppfile;
};

class grammar_wrapper : public grammar
{
public:
	void simple_grammar();
private:
    void init_funcs(std::vector<std::string>& funcs);
};

void simplegrammar_test::init_grammar(grammar& g)
{
	grammar_wrapper* p = reinterpret_cast<grammar_wrapper*>(&g);
	p->simple_grammar();
}

/* tiny grammar
 *
 * Program -> AProgramItem | AProgramItem Program
 * AProgramItem -> Function | AExpression ;
 * ValueDeclear -> type symbol
 * Expressions -> AExpression ; Expressions | AExpression ;
 * AExpression -> Assignment | FuncCall | PrintFunc | ValueDeclear
 * Assignment -> symbol = AValue | symbol = AValue op AValue
 * AValue -> symbol | number
 * FuncCall -> symbol = symbol ( AValue , AValue )
 * PrintFunc -> print ( AValue )
 * Function -> FunctionBegin FunctionContent ReturnExp }
 * FunctionHeader -> type symbol(type symbol, type symbol)
 * FunctionContent -> Expressions | Empty
 * ReturnExp -> return AValue;
 * FunctionBegin -> FunctionHeader {
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
	symbol_holder& syms = symbols_;
	production_holder& prods = productions_;

    std::vector<std::string> funcs;
    init_funcs(funcs);

	whitespaces_ = " \t\r\n";
	start_symbol_ = 0; // start symbol
	dthenu_ = true;

	const int unTermCount = 23;
	seperators_ = "{}()=*+-/;,";
	syms.resize(23 + seperators_.size());
	syms[0] = Asymbol("Program", 0);
	syms[1] = Asymbol("AProgramItem", 0);
	syms[2] = Asymbol("ValueDeclear", 0);
	syms[3] = Asymbol("Function", 0);
	syms[4] = Asymbol("Expressions", 0);
	syms[5] = Asymbol("Valuetype", 0);
	syms[6] = Asymbol("symbol", smac, "[a-zA-Z_][a-zA-Z_0-9]*");
	syms[7] = Asymbol("AExpression", 0);
	syms[8] = Asymbol("AValue", 0);
	syms[9] = Asymbol("number", smac, "([0-9]+)|([0-9]+\\.[0-9]*)|(\\.[0-9]+)");
	syms[10] = Asymbol("FuncCall", 0);
	syms[11] = Asymbol("Op", 0);
	syms[12] = Asymbol("PrintFunc", 0);
	syms[13] = Asymbol("print", keyword, "print");
	syms[14] = Asymbol("FunctionContent", 0);
	syms[15] = Asymbol("ReturnExp", 0);
	syms[16] = Asymbol("return", keyword, "return");
	syms[17] = Asymbol("int", keyword, "int");
	syms[18] = Asymbol("float", keyword, "float");
	syms[19] = Asymbol("", other_sym, "eplison");
	syms[20] = Asymbol("Assignment", 0);
	syms[21] = Asymbol("FunctionHeader", 0);
	syms[22] = Asymbol("FunctionBegin", 0);
	for(size_t i = 0; i < seperators_.size(); ++ i)
		syms[i + unTermCount] = Asymbol(std::string(1, seperators_[i]), sep, tstring(1, seperators_[i]));
	this->eplison_symbol_ = 19;
	// reset symbols name string
	{
		foreach (symbol& s, syms.begin(), syms.end())
		{
			if (s.name == NULL) continue;
			size_t size_in_bytes = sizeof(tchar) * s.Lname + 1;
			std::auto_ptr<tchar> buffer(alloc_.allocate(s.Lname + 1));
			memcpy(buffer.get(), s.name, size_in_bytes);
			s.name = buffer.release();
		}
		
		this->make_index();
	}

	// set keywords and statemachines
	{
		keywords_.resize(Asymbol::keywords.size());
		smacs_.resize(Asymbol::smacs.size());
		std::copy(Asymbol::keywords.begin(), Asymbol::keywords.end(), keywords_.begin());
		std::copy(Asymbol::smacs.begin(), Asymbol::smacs.end(), smacs_.begin());
	}
	
	prods.resize(29);
	symindex::tinyg() = this;
	//V::tinyg() = this;
	prods[0] = Aproduction(0, V(1), funcs[0]); // program -> aprogramitem
	prods[1] = Aproduction(0, V(1, 0), funcs[1]); // program -> aprogramitem program
	prods[2] = Aproduction(1, V(3), funcs[2]); // aprogramitem -> funcion
	prods[3] = Aproduction(1, V(7, ";"), funcs[3]); // aprogramitem -> AExpression ;
	prods[4] = Aproduction(2, V(5, 6), funcs[4]); // ValueDeclear -> Valuetype symbol
	prods[5] = Aproduction(4, V(7, ";", 4), funcs[5]); // Expressions -> AExpression ; Exprssions
	prods[6] = Aproduction(4, V(7, ";"), funcs[6]); // Expressions -> AExpression ;
	prods[7] = Aproduction(7, V(2), funcs[7]); // AExpression -> ValueDeclear
	prods[8] = Aproduction(7, V(10), funcs[8]); // AExpression -> FuncCall
	prods[9] = Aproduction(7, V(12), funcs[9]); // AExpression -> PrintFunc
	prods[10] = Aproduction(7, V(20), funcs[10]); // AExpression -> Assignment
	prods[11] = Aproduction(20, V(6, "=", 8), funcs[11]); // Assignment -> symbol = AValue
	prods[12] = Aproduction(20, V(6, "=", 8, 11, 8), funcs[12]); // Assignment -> symbol = AValue Op AValue
	prods[13] = Aproduction(8, V(6), funcs[13]); // AValue -> symbol
	prods[14] = Aproduction(8, V(9), funcs[14]); // AValue -> number
	prods[15] = Aproduction(10, V(6, "=", 6, "(", 6, ",", 6, ")"), funcs[15]); // FuncCall -> symbol = symbol ( symbol , symbol )
	prods[16] = Aproduction(12, V(13, "(", 8, ")"), funcs[16]); // PrintFunc -> print ( AValue )
	prods[17] = Aproduction(3, V(22, 14, 15, "}"), funcs[17]); // Function -> FunctionBegin FunctionContent ReturnExp }
	prods[18] = Aproduction(14, V(4), funcs[18]); //FunctionContent -> Expressions
	prods[19] = Aproduction(14, V(19), funcs[19]); // FunctionContent -> 
	prods[20] = Aproduction(15, V(16, 8, ";"), funcs[20]); // ReturnExp -> return AValue ;
	prods[21] = Aproduction(5, V(17), funcs[21]); // Valuetype -> int
	prods[22] = Aproduction(5, V(18), funcs[22]); // Valuetype -> float
	prods[23] = Aproduction(11, V("+"), funcs[23]); // Op -> +
	prods[24] = Aproduction(11, V("-"), funcs[24]); // Op -> -
	prods[25] = Aproduction(11, V("*"), funcs[25]); // Op -> *
	prods[26] = Aproduction(11, V("/"), funcs[26]); // Op -> /
	prods[27] = Aproduction(21, V(5, 6, "(", 5, 6, ",", 5, 6, ")"), funcs[27]); // FunctionHeader -> ValueType symbol ( ValueType symbol , ValueType symbol )
	prods[28] = Aproduction(22, V(21, "{"), funcs[28]); // FunctionBegin -> FunctionHeader {
}

void grammar_wrapper::init_funcs(std::vector<std::string>& funcs)
{
    funcs.resize(28);
    funcs[4] = "{ result->tflag = isv; result->v = regist(metas[0], metas[1]); }";
    funcs[12] = "{ four_tuple(metas[0], metas[1], metas[2], metas[3], metas[4]); }";
    funcs[14] = "{ result->v = tovalue(metas[0]); }";
    funcs[15] = "{ func_call(metas[0], metas[2], metas[4], metas[6]); }";
    funcs[16] = "{ func_call(NULL, metas[0], metas[2])}";
    funcs[17] = "{ regist_fun(metas[0], metas[1], metas[3], metas[6], metas[11], metas[12]); }";
    funcs[20] = "{ return_func(metas[1]); }";
    funcs[27] = "{ regist_fun(metas[0], metas[1], metas[3], metas[6], metas[11], metas[12]); }";
}

std::deque<tchar> Asymbol::stringBuf;
std::deque<std::pair<tstring, int32> > Asymbol::keywords;
std::deque<kog::triple<tstring, int32, bool> > Asymbol::smacs;

NEW_UNITTEST(simplegrammar_test);
