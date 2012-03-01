// type.h: types
//

#ifndef _TYPES_H_SC_
#define _TYPES_H_SC_

NAMESPACE_BEGIN(compile)

struct type
{
	int32 tid; // type id

	type(int32 id = -1)
		: tid(id)
	{}
};

NAMESPACE_END(compile)
#endif
