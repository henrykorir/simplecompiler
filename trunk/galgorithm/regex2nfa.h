// regex2nfa.h: regex string -> nfa tinygrammar
//

#ifndef _REGEX2NFA_H_SC_
#define _REGEX2NFA_H_SC_

#include <macros.h>
#include "grammaralgorithm.h"

NAMESPACE_BEGIN(compile)
NAMESPACE_BEGIN(ga)

class regex2nfa : public grammar_algorithm
{
public:
	regex2nfa(const tstring& regexstr, tinygrammar& otput)
		: grammar_algorithm("regex2nfa")
		, regexstr_(regexstr)
		, gout_(&otput)
	{
	}
public:
	/* overwrite */ virtual void invoke()
	{
		(*this)(regexstr_, *gout_);
	}
private:
	void operator()(const tstring& input, tinygrammar& otput);
	void tocfg(const tstring& input, tinygrammar& otput);
	void tonfa(const tinygrammar& input, tinygrammar& otput);
private:
	tstring regexstr_;
	tinygrammar* gout_;
};

NAMESPACE_END(ga)
NAMESPACE_END(compile)
#endif
