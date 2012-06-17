// iteratorX.h :
//

#ifndef _ITERATORX_H_HUXL_
#define _ITERATORX_H_HUXL_

#include <iterator>
#include <stdexcept>

#include <macros.h>
#include <mplX.h>

NAMESPACE_BEGIN(kog)

NAMESPACE_BEGIN(_ext)

template<size_t D> struct Diff_Mast_Grate_Than_Zero{};
template<> struct Diff_Mast_Grate_Than_Zero<0>;

NAMESPACE_END(_ext)

template<typename _Tx, int _Diff = 1>
struct array_iterator : public std::iterator<std::random_access_iterator_tag, _Tx*>
{
public:
	typedef std::iterator<std::random_access_iterator_tag, _Tx> _Base;
	typedef typename _Base::iterator_category iterator_category;
	typedef typename _Base::value_type value_type;
	typedef typename _Base::difference_type difference_type;
	typedef typename _Base::difference_type distance_type;	// retained
	typedef typename _Base::pointer pointer;
	typedef typename _Base::reference reference;
	typedef array_iterator<_Tx> _Myt;

	typedef array_iterator<typename mpl::remove_const<_Tx>::type> nonconst_type;
	typedef array_iterator<typename mpl::add_const<_Tx>::type> const_type;

//#ifdef MSVCR
//	typedef _STD _Range_checked_iterator_tag _Checked_iterator_category;
//#endif

public:


	typedef mpl::fire_error<_ext::Diff_Mast_Grate_Than_Zero<_Diff> > _Diff_GT_0;
public:
	array_iterator(_Tx* p, distance_type b, distance_type f)
		: _ptr(p), _before(b), _follow(f)
	{
		assert_invalidate();
	}

	array_iterator(_Tx*p, distance_type size)
		: _ptr(p), _before(0), _follow(size)
	{
		assert_invalidate();
	}

	array_iterator(const nonconst_type& aIter)
		: _ptr(aIter._ptr)
		, _before(aIter._before)
		, _follow(aIter._follow)
	{}
	
	array_iterator()
		: _ptr(NULL), _before(0), _follow(0)
	{}

public:
	//operator const _Tx*() const { return _ptr; }
public:
	reference operator*() const { return *_ptr; }
	pointer operator->() const { return _ptr; }

	reference operator[](distance_type _Dis) { return _ptr[_Dis]; }

public:
	_Tx* get() const { return _ptr; }
public:
	_Myt& operator++() { return forward(*this); }
	_Myt& operator--() { return backward(*this); }
	_Myt operator++(int) { _Myt temp(*this); forward(*this); return temp;}
	_Myt operator--(int) { _Myt temp(*this); backward(*this); return temp; }

public:
	_Myt operator +(distance_type _Dis) const
	{
		_Myt buf(*this);
		return buf += _Dis;
	}

	_Myt operator -(distance_type _Dis) const
	{
		_Myt buf(*this);
		return buf -= _Dis;
	}

	distance_type operator -(const _Myt& _PY) const
	{
		return (_ptr - _PY._ptr) / _Diff;
	}

	_Myt& operator +=(distance_type _Dis)
	{
		return forward(*this, _Dis);
	}

	_Myt& operator -=(distance_type _Dis)
	{
		return backward(*this, _Dis);
	}

	friend _Myt operator+ (distance_type _Dis, const _Myt& _Iter)
	{
		return _Iter + _Dis;
	}

public:
	bool operator == (const _Myt& _Other) const
	{
		return _ptr == _Other._ptr;
	}

	bool operator != (const _Myt& _Other) const
	{
		return _ptr != _Other._ptr;
	}

	bool operator <(const _Myt& _Other) const
	{
		return _ptr < _Other._ptr;
	}

	bool operator <=(const _Myt& _Other) const
	{
		return _ptr <= _Other._ptr;
	}

	bool operator >(const _Myt& _Other) const
	{
		return _ptr < _Other._ptr;
	}

	bool operator >=(const _Myt& _Other) const
	{
		return _ptr <= _Other._ptr;
	}

protected:
	static array_iterator& forward(array_iterator& aniterator, distance_type dis = 1)
	{
		aniterator._ptr += _Diff * dis;
		aniterator._follow -= dis;
		aniterator._before += dis;
		return aniterator.assert_invalidate();
	}

	static array_iterator& backward(array_iterator& aniterator, distance_type dis = 1)
	{
		aniterator._ptr -= _Diff * dis;
		aniterator._follow += dis;
		aniterator._before -= dis;
		return aniterator.assert_invalidate();
	}

	array_iterator& assert_invalidate()
	{
		((const array_iterator&)(*this)).assert_invalidate();
		return *this;
	}

	const array_iterator& assert_invalidate() const
	{
		const difference_type mask = ((difference_type)1)
			<< ((sizeof(difference_type) * 8) - 1);
		if((_before & mask) != 0)
			throw std::range_error("before begin!");
		else if((_follow & mask) != 0)
			throw std::range_error("after end!");
		return *this;
	}

public:
	_Tx* _ptr;
	difference_type _before;
	difference_type _follow;
};

template<typename _Iter1, typename _Iter2>
struct iterator_pair : public std::pair<_Iter1, _Iter2>
{
	typedef iterator_pair<_Iter1, _Iter2> _Myt;
	typedef std::pair<_Iter1, _Iter2> _BaseType;

	iterator_pair()
	{}

	iterator_pair(_Iter1 _It1, _Iter2 _It2)
		: _BaseType(_It1, _It2)
	{}

	template<typename _Other1, typename _Other2>
	iterator_pair(const std::pair<_Other1, _Other2>& _Right)
		: _BaseType(_Right)
	{}

public:
	_Myt& operator++() { return forward(); }
	_Myt& operator--() { return backward(); }
	_Myt operator++(int) { _Myt temp(*this); forward(); return temp;}
	_Myt operator--(int) { _Myt temp(*this); backward(); return temp; }

	bool operator == (const _Myt& _Other) const
	{
		return _BaseType::first == _Other.first && _BaseType::second == _Other.second;
	}

	bool operator != (const _Myt& _Other) const
	{
		return !(*this == _Other);
	}
protected:
	_Myt& forward()
	{
		++ _BaseType::first;
		++ _BaseType::second;
		return *this;
	}

	_Myt& backward()
	{
		-- _BaseType::first;
		-- _BaseType::second;
		return *this;
	}
};

template<typename _Iter>
struct iterator_wrap_t
{
    iterator_wrap_t(_Iter _First, _Iter _End)
        : Current_(_First)
        , Last_(_End)
    {
    }

    bool next()
    {
        if (Current_ != Last_)
        {
            ++ Current_;
            return true;
        }
        else
        {
            return false;
        }
    }

    typename _Iter::reference operator*() const
    {
        return *Current_;
    }

    typename _Iter::pointer operator->() const
    {
        return Current_.operator->();
    }

    _Iter Current_;
    _Iter Last_;
};

template<typename _Class> iterator_wrap_t<typename _Class::const_iterator> wrap(const _Class& obj)
{
    return iterator_wrap_t<typename _Class::const_iterator>(obj.begin(), obj.end());
}

template<typename _Class> iterator_wrap_t<typename _Class::iterator> wrap(const _Class& obj)
{
    return iterator_wrap_t<typename _Class::iterator>(obj.begin(), obj.end());
}

template<int _Diff, typename _Tx>
array_iterator<_Tx, _Diff> array_begin(_Tx* _Begin_ptr, typename array_iterator<_Tx, _Diff>::difference_type _Length)
{
	return array_iterator<_Tx, _Diff>(_Begin_ptr, 0, _Length);
}

template<int _Diff, typename _Tx>
array_iterator<_Tx, _Diff> array_end(_Tx* _Begin_ptr, typename array_iterator<_Tx, _Diff>::difference_type _Length)
{
	return array_iterator<_Tx, _Diff>(_Begin_ptr + _Length, _Length, 0);
}

template<typename _Iter>
_Iter iterator_next(_Iter iter)
{
	return ++ iter;
}

template<typename _Iter>
_Iter iterator_prior(_Iter iter)
{
	return -- iter;
}

NAMESPACE_END(kog)

#endif

