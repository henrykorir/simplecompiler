
// basic grammars: easy and import algorithm
//

#ifndef _BASIC_GRAMMARS_H_SC_
#define _BASIC_GRAMMARS_H_SC_

#include "grammaralgorithm.h"

NAMESPACE_BEGIN(compile)
NAMESPACE_BEGIN(ga)

class firstset : public grammar_algorithm
{
public:
	typedef kog::smart_vector<int32> vecint;
	typedef kog::smart_vector<vecint> vecintset;
public:
	firstset(const tinygrammar& gin, vecintset& sets)
	: grammar_algorithm("get first set")
	, gin_(&gin)
	, sets_(&sets)
	{}

public:
	/* overwrite */ virtual void invoke()
	{
		(*this)(*gin_, *sets_);
	}
private:
	void operator()(const tinygrammar& gin, vecintset& sets);

	const tinygrammar* gin_;
	vecintset* sets_;
};

class followset : public grammar_algorithm
{
public:
	typedef kog::smart_vector<int32> vecint;
	typedef kog::smart_vector<vecint> vecintset;
public:
	followset(const tinygrammar& gin, const vecintset& firstsets, vecintset& followsets)
	: grammar_algorithm("get follow set")
	, gin_(&gin)
	, firstsets_(&firstsets)
	, followsets_(&followsets)
	{}

public:
	/* overwrite */ virtual void invoke()
	{
		(*this)(*gin_, *firstsets_, *followsets_);
	}
private:
	void operator()(const tinygrammar& gin, const vecintset& firstsets, vecintset& followsets);

	const tinygrammar* gin_;
	const vecintset* firstsets_;
	vecintset* followsets_;
};

NAMESPACE_END(ga)
NAMESPACE_END(compile)

#endif
