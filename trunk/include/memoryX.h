// memoryX.h: define memory structs and functions
//

#ifndef _MEMORYX_H_HUXL_
#define _MEMORYX_H_HUXL_

#include <macros.h>
#include <iteratorX.h>

NAMESPACE_BEGIN(kog)

template<typename _Tx>
struct varray
{
public:
	typedef varray<_Tx> _Myt;
public:
	varray(size_t _L)
		: objects_(new _Tx(_L))
		, length_(_L)
		, ref_count_(once_cont())
	{}
	
	varray(_Myt& other)
		: length_(other.length_)
		, ref_count_(inc_count(other))
		, objects_(other.objects_)
	{}

private:
	static int* once_count()
	{
		static int __once = 1;
		return &__once;
	}

	static int* inc_count(_Myt& other)
	{
		if ()
		{
		}
	}
private:
	size_t length_;
	_Tx* objects_;
	int* ref_count_;
};

NAMESPACE_END(kog)

#endif
