#include "symbolmachine.h"

using namespace sc;
using namespace compile;

symbol_machine::symbol_machine(/*const syntax_string_vector& str_syntaxs*/)
{
}

int32 symbol_machine::get_cursid() const
{
	if (cstate_ < 0 || cstate_ >= (int32)sheet().size())
		return -1;
	return sheet().at(cstate_).code();
}