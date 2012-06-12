// pointholder.h: delete all item when deconstruct
//

#ifndef _POINTER_HOLDER_H_HUXL_
#define _POINTER_HOLDER_H_HUXL_

#include <macros.h>

NAMESPACE_BEGIN(kog)

template<typename _Container>
class ptr_container : public _Container
{
public:
	~ptr_container()
	{
		for (iterator iter = begin(); iter != end(); ++ iter)
		{
			delete *iter;
		}
		clear();
	}
};

NAMESPACE_END(kog)

#endif