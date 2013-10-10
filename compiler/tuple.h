// tuple.h: 
//

#ifndef _TUPLE_H_SC_
#define _TUPLE_H_SC_

#include "object.h"
#include "operation.h"
#include "variable.h"

NAMESPACE_BEGIN(compile)
NAMESPACE_BEGIN(runtime)

struct tuple : public variable
{
};

struct four_tuple : public tuple
{
    operations::op oper;  
    variable* dst;
    variable* src1;
    variable* src2;
};

NAMESPACE_END(runtime)
NAMESPACE_END(compile)

#endif
