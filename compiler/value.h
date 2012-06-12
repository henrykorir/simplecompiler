// value.h: values
//

#ifndef _VALUES_H_SC_
#define _VALUES_H_SC_

#include <macros.h>
#include <basic_types.h>

#include "object.h"

NAMESPACE_BEGIN(compile)
NAMESPACE_BEGIN(runtime)

struct value : public object
{
    union {
        uint32 address;
        byte* initv;
    };
    uint32 size;
};

NAMESPACE_END(runtime)
NAMESPACE_END(compile)

#endif
