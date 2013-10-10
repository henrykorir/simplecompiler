// eclosures.h : e-closures
//

#ifndef _ECLOSURE_H_SC_
#define _ECLOSURE_H_SC_

#include <macros.h>
#include <stringX.h>
#include <arrayX.h>
#include "grammaralgorithm.h"

NAMESPACE_BEGIN(compile)
NAMESPACE_BEGIN(ga)

class eclosure : public grammar_algorithm
{
public:
	typedef kog::smart_vector<int32> closure;
	typedef kog::smart_vector<closure> closure_array;
public:
	eclosure(const tinygrammar& tig, closure_array& ecls)
		: grammar_algorithm("e-closure")
		, tig_(&tig)
		, ecs_(&ecls)
	{}
public:
	/* overwrite */ virtual void invoke()
	{
		return (*this)(*tig_, *ecs_);
	}

private:
	void operator()(const tinygrammar& tig, closure_array& closures);

	const tinygrammar* tig_;
	closure_array* ecs_;
};

NAMESPACE_END(ga)
NAMESPACE_END(compile)

#endif
