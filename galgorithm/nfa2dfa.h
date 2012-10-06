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
	// because this algorithm will move all terminate symbol before all non-terminate symbol
	// terminate symbol's sid will changed.
	// so, please let all termimate's(exclude eplison, it will remove by algorithm) sid is less than non-terminate symbol
	nfa2dfa(const tinygrammar& gin, tinygrammar& gout)
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
	void operator()(const tinygrammar& input, tinygrammar& otput);

	void todfa(const tinygrammar& tig, tinygrammar& tog);
	void mini_status(const tinygrammar& tig, tinygrammar& tog);
private:
	//friend struct context;
	//friend struct minstatus;
	// make sure the input tinygrammar is right tinygrammar
	void make_sure_rg(const tinygrammar& input);

	void split(const std::vector<const production*>& t, kog::smart_vector<int32>& smap, std::deque<std::set<int32> >& s) const;
	const tinygrammar* gin_;
	tinygrammar* got_;
};

NAMESPACE_END(ga)
NAMESPACE_END(compile)
#endif

