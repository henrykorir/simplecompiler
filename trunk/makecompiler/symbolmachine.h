// symbolmachine.h : 
//

#ifndef _SYMBOL_MACHINE_H_SC_
#define _SYMBOL_MACHINE_H_SC_

#include "statemachine.h"
#include <functionalX.h>

NAMESPACE_BEGIN(compile);

class symbol_machine : public state_machine
{
public:
	typedef kog::triple<tstring, int32, bool> syntax_string;
	typedef kog::smart_vector<syntax_string> syntax_string_vector;
public:
	symbol_machine();
	symbol_machine(const syntax_string str_syntaxs[], size_t N);
public:
	int32 get_cursid() const;
private:
	syntax_string_vector sstr_;
};

NAMESPACE_END(compile);

#endif