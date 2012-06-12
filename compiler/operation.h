// operator.h: operations
//

#ifndef _OPERATIONS_H_SC_
#define _OPERATIONS_H_SC_

#include <macros.h>
#include "object.h"

NAMESPACE_BEGIN(compile)
NAMESPACE_BEGIN(runtime)
NAMESPACE_BEGIN(operations)

enum op
{
	plus,
	minus,
	multi,
	divid,
	assign,
	func_call,
    lt,
    gt,
    le,
    ge,
    eq,
    ne,
    newv,
    func_ret,
    internal
};

NAMESPACE_END(operations)

struct operation : object
{
    operation(operations::op _oper)
        : oper(_oper)
    {}

    operations::op oper;
};

NAMESPACE_END(runtime)
NAMESPACE_END(compile)

#endif
