// share_ptr.h: share pointer
//

#ifndef _SHARE_PTR_H_HUXL_
#define _SHARE_PTR_H_HUXL_

#include <macros.h>
#include <memory>

NAMESPACE_BEGIN(kog)

template<typename _Tx>
class share_ptr
{
public:
	typedef _Tx element_type;
public:
	explicit share_ptr(_Tx* p = NULL) throw()
		: refCount(p != NULL ? (new int(1)) : NULL)
		, ptr(p)
	{
	}

	template<typename _Ty>
	share_ptr(const share_ptr<_Ty>& other) throw()
		: refCount(other.refCount)
		, ptr(other.ptr)
	{
		inc_holder();
	}

	~share_ptr()
	{
		free_holder();
	}
public:
	template<typename _Ty>
	share_ptr<_Tx>& operator=(const share_ptr<_Ty>& other) throw ()
	{
		if(ptr == other.ptr) return *this;
		free_holder();
		ptr = other.ptr;
		refCount = other.refCount;
		inc_holder();
		return *this;
	}
public:
	void reset(_Tx* p) throw()
	{
		if(p == ptr) return;
		free_holder();
		refCount = p ? new int(1) : NULL;
		ptr = p;
	}

	_Tx* get() throw()
	{
		return ptr;
	}

	void swap(share_ptr<_Tx>& other)
	{
		std::swap(ptr, other.ptr);
		std::swap(refCount, other.refCount);
	}
public:
	_Tx& operator*() const
	{
		return *((_Tx*)ptr);
	}

	_Tx* operator->() const
	{
		return (_Tx*)ptr;
	}
private:
	void free_holder()
	{
		if(refCount && !(-- *refCount))
		{
			delete ptr;
			delete refCount;
		}
		ptr = NULL;
		refCount = NULL;
	}

	void inc_holder()
	{
		if(refCount) ++*refCount;
	}
private:
	int* refCount;
	const _Tx* ptr;
};

NAMESPACE_END(kog)

#endif
