// heapX.h: 
//

#ifndef _HEAPX_H_HUXL_
#define _HEAPX_H_HUXL_

#include <macros.h>
#include <functional>
#include <algorithm>

NAMESPACE_BEGIN(kog)

template<typename _C, typename _Less>
class heap
{
public:
	typedef typename _C::value_type value_type;
	typedef typename _C::iterator iterator;
	typedef typename _C::const_iterator const_iterator;
public:
	heap()
	{}

	template<typename _Iter>
	heap(_Iter _First, _Iter _Last)
	{}

	~heap()
	{}
public:
	template<typename _Iter>
	void insert(_Iter _First, _Iter _Last)
	{
		c_.insert(c_.end(), _First, _Last);
		update();
	}

	void insert(const value_type& _v)
	{
		c_.push_back(_v);
		update();
	}

public:
	void remove(const value_type& _v)
	{
		
		update();
	}

	void earse(iterator ei)
	{
		c_.earse(ei);
		update();
	}

	void earse(iterator _First, iterator _Last)
	{
		c_.earse(_First, _Last);
		update();
	}

public:
	const_iterator find(const value_type& _v) const
	{
	}

	iterator find(const value_type _v)
	{
	}

	bool isexist(const value_type& _v) const
	{
		return find(_v) != c_.end();
	}

protected:
	void update()
	{
		_Less less;
		std::make_heap(c_.begin(), c_.end(), less);
	}
public:
	// iterator
	iterator begin() { return c_.begin(); }
	iterator end() { return c_.end(); }
	const_iterator begin() const { return c_.begin(); }
	const_iterator end() const { return c_.end(); }
protected:
	_C c_;
};

NAMESPACE_END(kog)

#endif
