// lranalyse.h : test whea a tinygrammar is lr(1) tinygrammar, and make the lrmachine for the given tinygrammar
//

#ifndef _LRANALYSE_H_SC_
#define _LRANALYSE_H_SC_

#include <macros.h>

#include "grammaralgorithm.h"
#include <grammar.h>
#include <lrmachine.h>

struct AlgorithmArg;
NAMESPACE_BEGIN(compile)
NAMESPACE_BEGIN(ga)

class lranalyse : public grammar_algorithm
{
public:
	lranalyse(const tinygrammar& ing, lrmachine& lrm)
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
	void operator()(const tinygrammar& gin, lrmachine& mot);
	
	void make_true_lr1(const tinygrammar& tig) const;

	void make_machine(AlgorithmArg& arg, const tinygrammar& tig, lrmachine& mot) const;
private:
	const tinygrammar* gin_;
	lrmachine* mot_;
};

NAMESPACE_END(ga)
NAMESPACE_END(compile)
#endif
