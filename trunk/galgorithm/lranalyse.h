// lranalyse.h : test whea a grammar is lr(1) grammar, and make the lrmachine for the given grammar
//

#ifndef _LRANALYSE_H_SC_
#define _LRANALYSE_H_SC_

#include <macros.h>

#include "grammaralgorithm.h"
#include <grammar.h>
#include <lrmachine.h>

NAMESPACE_BEGIN(compile)
NAMESPACE_BEGIN(ga)

class lranalyse : public grammar_algorithm
{
public:
	lranalyse(const grammar& ing, lrmachine& lrm)
		: grammar_algorithm("lr analyse")
		, gin_(&ing)
		, mot_(&lrm)
	{
	}
public:
	/* overwrite */ virtual void invoke()
	{
		(*this)(*gin_, *mot_);
	}
private:
	void operator()(const grammar& gin, lrmachine& mot);
	
	void make_true_lr1(const tinygrammar& tig) const;
private:
	const grammar* gin_;
	lrmachine* mot_;
};

NAMESPACE_END(ga)
NAMESPACE_END(compile)
#endif
