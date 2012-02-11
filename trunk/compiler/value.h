// value.h: values
//

#ifndef _VALUES_H_SC_
#define _VALUES_H_SC_

#include <macros.h>
#include <basic_types.h>

NAMESPACE_BEGIN(compile)

struct value
{
	const type* vtype;
	const symbol* vsym;
	uint32 address;
	uint32 size;
	byte* defvalue;
};

NAMESPACE_END(compile)

#endif
