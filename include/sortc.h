// sortc.h: 
//

#ifndef _SORTC_H_HUXL_
#define _SORTC_H_HUXL_

#include <macros.h>

NAMESPACE_BEGIN(kog)
template<typename _C, typename _Pred>
class sortc
{
public:
	typedef typename _C::value_type value_type;
	typedef typename _C::iterator iterator;
	typedef typename _C::const_iterator const_iterator;
public:
	sortc()
	{}

	sortc(const sortc<_C, _Pred>& _Other)
	: c_(_Other.c_)
	{}

	template<typename _Iter>
	sortc(_Iter _First, _Iter _Last)
	: c_(_First, _Last)
	{
	}

public:
	// iterator
	iterator begin() { return c_.begin(); }
	iterator end() { return c_.end(); }
	const_iterator begin() const { return c_.begin(); }
	const_iterator end() const { return c_.end(); }
public:
	void insert(const value_type& v)
	{
		c_.insert(c_.end(), v);
		update();
	}

	template<typename _Iter>
	void insert(_Iter _First, _Iter _Last)
	{
		c_.insert(c_.end(), _First, _Last);
		update();
	}
public:
	iterator erase(iterator ei)
	{
		return c_.erase(ei);
	}

	iterator erase(iterator _First, iterator _Last)
	{
		return c_.erase(_First, _Last);
	}
public:
	iterator find(const value_type& v)
	{
		_Pred cmp;
		iterator iter = lower_bound(v);
		if(iter != end() && v == *iter)
			return iter;
		return end();
	}

	const_iterator find(const value_type& v) const
	{
		_Pred cmp;
		const_iterator iter = lower_bound(v);
		if(iter != end() && v == *iter)
			return iter;
		return end();
	}

	bool exist(const value_type& v) const
	{
		return find(v) != v.end();
	}

	iterator lower_bound(const value_type& v)
	{
		return std::lower_bound(begin(), end(), v, _Pred());
	}

	const_iterator lower_bound(const value_type& v) const
	{
		return std::lower_bound(begin(), end(), v, _Pred());
	}

	iterator upper_bound(const value_type& v)
	{
		return std::upper_bound(begin(), end(), v, _Pred());
	}

	const_iterator upper_bound(const value_type& v) const
	{
		return std::upper_bound(begin(), end(), v, _Pred());
	}
public:
	bool empty() const { return c_.empty(); }
	size_t size() const { return c_.size(); }	
	void swap(sortc<_C, _Pred>& _Other)
	{
		c_.swap(_Other.c_);
	}

//	void merge(sortc<_C, _Pred>& _Other)
//	{
//		
//	}

protected:
	void update()
	{
		std::sort(begin(), end(), _Pred());
		// unique
		c_.erase(std::unique(c_.begin(), c_.end()), c_.end());
	}
protected:
	_C c_;
};

NAMESPACE_END(kog)
#endif
