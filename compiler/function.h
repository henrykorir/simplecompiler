// function.h: 
//

#ifndef _FUNCTION_H_SC_
#define _FUNCTION_H_SC_

#include <macros.h>
#include "type.h"
#include "variable.h"

NAMESPACE_BEGIN(compile)
NAMESPACE_BEGIN(runtime)
struct funcparamlist;

class function : public variable
{
public:
	//function()
	const type* return_type;
private:
	funcparamlist* params_;
};

NAMESPACE_END(runtime)
NAMESPACE_END(compile)

#endif
