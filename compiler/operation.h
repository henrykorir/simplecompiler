// operator.h: operations
//

#ifndef _OPERATIONS_H_SC_
#define _OPERATIONS_H_SC_

#include <macros.h>
#include <scerror.h>
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

struct op_string
{
private:
    static const _Str* enum_string()
    {
        const static _Str _names[] = 
        {
            "plus", "minus", "multi", "divid", "assign",
            "func_call", "lt", "gt", "le", "ge", "eq",
            "ne", "newv", "func_ret", "internal"
        };
        return _names;
    }

public:
    static const _Str& to_string(op i)
    {
        if (i < plus || i > internal)
            fire("invalidate operation!");
        return enum_string()[i];
    }

    static op parse(const _Str& str)
    {
        for (op i = plus; i <= internal; i = (op)(i + 1))
        {
            if (to_string(i) == str)
                return i;
        }
        fire("can't parse " + str + " to operator!");
    }
};

NAMESPACE_END(operations)

struct operation : object
{
    operation(operations::op _oper)
        : oper(_oper)
    {}

    operation(const _Str& str)
        : oper(operations::op_string::parse(str))
    {}

    /* overwrite */ virtual _Str to_string() const
    {
        return operations::op_string::to_string(oper);
    }

    operations::op oper;
};

NAMESPACE_END(runtime)
NAMESPACE_END(compile)

#endif
