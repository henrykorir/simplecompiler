// lrmachine.h : LALR(1) machine
//

#ifndef _LALR1_MACHINE_H_SC_
#define _LALR1_MACHINE_H_SC_

#include <stack>
#include <list>
#include <macros.h>
#include "automachine.h"
#include "grammar.h"

NAMESPACE_BEGIN(compile)

class lrmachine : public automachine
{
	struct stackitem
	{
		int32 state;
		int32 meta;

		stackitem(int32 s = -1, int32 m = -1)
			: state(s)
			, meta(m)
		{}
	};

	typedef std::stack<stackitem> analysestack;
//	typedef kog::smart_vector<lrstate> lrstatearray;
	typedef kog::smart_vector<int32> int32array;
public:
	struct pinfo
	{
		int32 pid;
		int32 dot;
	};
public:
	typedef kog::smart_vector<symfunc*> funcarray;
	typedef kog::smart_vector<pinfo> pinfoarray;
public:
	lrmachine()
	: pg_(NULL)
	{}

	lrmachine(const tinygrammar& g)
	: pg_(&g)
	{}
public:
	/* overwrite */ virtual void init();
	/* overwrite */ virtual bool eta(int meta);
public:
	std::list<pinfoarray>& morelist() 
	{
		return morelist_;
	}

	const tinygrammar& tinyg() const
	{
		return *pg_;
	}
public:
	enum { accept_state = 0}; // using state 0 as ending state
protected:
	// reduce using production[pid]
	int32 reduce(int32 pid);

public:
	void swap(lrmachine& other) throw();
private:
	analysestack pstack_;
	funcarray funcs_;
//	lrstatearray lrs_;
	const tinygrammar* pg_;
	std::list<pinfoarray> morelist_;
};

NAMESPACE_END(compile)
#endif
