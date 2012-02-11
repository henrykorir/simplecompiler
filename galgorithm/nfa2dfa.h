//nfa2dfa.h : nfa -> dfa
//

#ifndef _NFA_TO_DFA_H_SC_
#define _NFA_TO_DFA_H_SC_

#include "grammaralgorithm.h"
#include <statemachine.h>
#include <vector>
#include <deque>
#include <set>

NAMESPACE_BEGIN(compile)
NAMESPACE_BEGIN(ga)

class nfa2dfa : public grammar_algorithm
{
public:
	nfa2dfa(const grammar& gin, grammar& gout)
	: grammar_algorithm("nfa2dfa")
	, gin_(&gin)
	, got_(&gout)
	{}

public:
	/* overwrite */ virtual void invoke()
	{
		return (*this)(*gin_, *got_);
	}

private:
	void operator()(const grammar& input, grammar& otput);

	void todfa(const tinygrammar& tig, tinygrammar& tog);
	void mini_status(const tinygrammar& tig, tinygrammar& tog);
private:
	//friend struct context;
	//friend struct minstatus;
	// make sure the input grammar is right grammar
	void make_sure_rg(const grammar& input);

	void split(const std::vector<const production*>& t, kog::smart_vector<int32>& smap, std::deque<std::set<int32> >& s) const;
	const grammar* gin_;
	grammar* got_;
};

NAMESPACE_END(ga)
NAMESPACE_END(compile)
#endif

