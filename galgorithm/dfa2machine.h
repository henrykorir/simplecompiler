//dfa2machine.h : dfa -> automachine
//

#ifndef _DFA_TO_AUTOMACHINE_H_SC_
#define _DFA_TO_AUTOMACHINE_H_SC_

#include "grammaralgorithm.h"
#include <statemachine.h>

NAMESPACE_BEGIN(compile)
NAMESPACE_BEGIN(ga)

class dfa2machine : public grammar_algorithm
{
public:
	dfa2machine(const tinygrammar& gin, automachine& mot)
	: grammar_algorithm("dfa2machine")
	, gin_(&gin)
	, mot_(&mot)
	{}

public:
	/* overwrite */ virtual void invoke()
	{
		(*this)(*gin_, *mot_);
	}

private:
	void operator()(const tinygrammar& input, automachine& otput);

	void make_sure_dfa(const tinygrammar& input) const;
	
	const tinygrammar* gin_;
	automachine* mot_;
};

NAMESPACE_END(ga)
NAMESPACE_END(compile)
#endif

