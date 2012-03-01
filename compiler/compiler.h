// compiler.h: compiler global value
//

#ifndef _COMPILER_H_SC_
#define _COMPILER_H_SC_

#include <map>
#include <list>
#include <singleton.h>
#include <arrayX.h>
#include <shared_ptr.h>
#include <buckethash.h>
#include <tree.h>
#include "automachine.h"
#include "statemachine.h"
#include "lalr1machine.h"

NAMESPACE_BEGIN(compile)
NAMESPACE_BEGIN(doc)

struct machine
{
	machine()
	: sid(-2)
	{}

	machine(kog::shared_ptr<compile::automachine> m, int32 id)
		: mac(m), sid(id)
	{}

	kog::shared_ptr<compile::automachine> mac;
	int32 sid;
};

NAMESPACE_END(doc)
NAMESPACE_END(compile)

struct string_2_int
{
	enum { max_int = 100 };
	int32 operator()(const std::string& s) const
	{
		const char* p = s.c_str();
		int32 v = 0;
		while(*p) v += *(p ++);
		return v % max_int;
	}
};

class compiler : public kog::singleton<compiler>
{
	friend struct split_separators;
public:
	typedef compile::automachine automachine;
	typedef compile::state_machine state_machine;
	typedef compile::lalr1machine lalr1machine;
public:
	compiler();
public:
	void initialization();
public:
	static state_machine get_number_machine();
	static state_machine get_symbol_machine();
	static state_machine get_string_machine();
	static int get_all_machines(std::list<compile::doc::machine>& mlist);
public:
	static bool is_separator(sc::int32 elem);
	void check(const std::string& fname);
private:
	int32 is_keywords(const std::string& s) const;
private:
	std::map<std::string, compile::doc::machine> machines;
	kog::buckethash<std::string, int32, string_2_int> keywords;
	kog::smart_vector<sc::int32> separators;
	kog::smart_vector<sc::int32> printablechars;
	kog::tree<int32> sepsid;
	compile::tinygrammar tg;
};

#endif
