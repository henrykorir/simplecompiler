// prodfunction.h : parse production function content
//

#ifndef _PROD_FUNCTION_H_SC_
#define _PROD_FUNCTION_H_SC_

#include "compilersyntax.h"

NAMESPACE_BEGIN(compile);
NAMESPACE_BEGIN(syntax);

class compiler_action_parser
{
public:
	compiler_action_parser(funcinfo_t& otput);
public:
	bool newline(const tstring& line) { return (*this)(line); }
	bool operator()(const tstring& line);
private:
	funcinfo_t* otput_;
};

NAMESPACE_END(syntax);
NAMESPACE_END(compile);

#endif
