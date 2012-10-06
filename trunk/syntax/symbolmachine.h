// symbolmachine.h : 
//

#ifndef _SYMBOL_MACHINE_H_SC_
#define _SYMBOL_MACHINE_H_SC_

#include "statemachine.h"
#include <vector>

NAMESPACE_BEGIN(compile);

class symbol_machine : public state_machine
{
public:
	typedef std::vector<std::pair<tstring, int32> > syntax_string_vector;
public:
	//symbolmachine();
	symbol_machine(/*const syntax_string_vector& str_syntaxs*/);
public:
	int32 get_cursid() const;
private:
	syntax_string_vector sstr_;
};

NAMESPACE_END(compile);

#endif