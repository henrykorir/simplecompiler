// grammaralgorithm.h: grammar algorithm
//

#ifndef _GRAMMAR_ALGORITHM_H_SC_
#define _GRAMMAR_ALGORITHM_H_SC_

#include <macros.h>
#include <basic_types.h>
#include <grammar.h>

NAMESPACE_BEGIN(compile)
NAMESPACE_BEGIN(ga) // namespace grammar algirothm

class grammar_algorithm
{
public:
	grammar_algorithm(const tstring& name)
		: name_(name)
	{
	}
public:
	virtual void invoke() = 0;
public:
	const tstring& name() const
	{
		return name_;
	}
private:
	tstring name_;
};

NAMESPACE_END(ga)
NAMESPACE_END(compile)
#endif

