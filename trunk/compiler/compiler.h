// compiler.h: compiler global value
//

#ifndef _COMPILER_H_SC_
#define _COMPILER_H_SC_

#include <map>
#include <list>
#include <singleton.h>
#include <arrayX.h>
#include "statemachine.h"

class compiler : public kog::singleton<compiler>
{
public:
	typedef compile::state_machine state_machine;
public:
	void initmachines();
public:
	static state_machine get_number_machine();
	static state_machine get_symbol_machine();
	static state_machine get_string_machine();
	static int get_all_machines(std::list<state_machine*>& mlist);
public:
	static bool is_separator(sc::int32 elem);
private:
	std::map<std::string, state_machine> machines;
	kog::smart_vector<sc::int32> separators;
	kog::smart_vector<sc::int32> printablechars;
};

#endif
