// arrayX.h :
//

#pragma once

#ifndef _ARRAYX_H_HUXL_
#define _ARRAYX_H_HUXL_

#include <macros.h>
#include <mplX.h>
#include <iteratorX.h>
#include <exception>

NAMESPACE_BEGIN(kog)

template<typename _Ty, size_t _Length>
struct SmartArray
{
public:
	typedef std::size_t size_type;
	typedef typename mpl::remove_const<_Ty>::type value_type;
	//typedef _Ty* iterator;
	//typedef const _Ty* const_iterator;
	//typedef stdext::checked_array_iterator<value_type*> iterator;
	//typedef stdext::checked_array_iterator<const value_type*> const_iterator;
	typedef array_iterator<value_type> iterator;
	typedef array_iterator<const value_type> const_iterator;

	typedef std::reverse_iterator<iterator> reverse_iterator;
	typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
	typedef SmartArray<_Ty, _Length> _Myt;
	typedef value_type& reference;
	typedef const value_type& const_reference;
	typedef _Ty* pointer;
	typedef const _Ty* const_pointer;

public:
	const static size_t Length = _Length;
public:
	SmartArray() {}

	explicit SmartArray(const_reference val)
	{
		iterator first = begin(), last = end();
		while(first != last) *(first ++) = val;
	}

	template<typename _Iter>
	SmartArray(_Iter _First, _Iter _Last)
	{
		for(size_t i = 0; i != _Length && _First != _Last; ++ i)
			_buf[i] = *(_First ++);
	}
public:
	static _Myt* attach(value_type anArray[_Length])
	{
		return (_Myt*)anArray;
	}

	static SmartArray<const value_type, _Length>* attach(const value_type anArray[_Length])
	{
		return (SmartArray<const value_type, _Length>*)anArray;
	}
public:
	size_t size() const {return _Length;}
public:
	iterator begin() { return iterator(_buf, _Length); }
	const_iterator begin() const { return const_iterator(_buf, _Length); }
	iterator end() { return iterator(_buf + _Length, _Length, 0); }
	const_iterator end() const { return const_iterator(_buf + _Length, _Length, 0); }

public:
	reverse_iterator rbegin() { return reverse_iterator(end()); }
	reverse_iterator rend() { return reverse_iterator(begin()); }
	const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
	const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }

public:
	pointer ptr() { return _buf; }
	const_pointer ptr() const { return _buf; }

	reference front() { return _buf[0]; }
	reference back() { return _buf[_Length - 1]; }

	const_reference front() const { return _buf[0]; }
	const_reference back() const { return _buf[_Length - 1]; }
public:
	_Myt& operator=(const _Ty& val)
	{
		iterator iter = begin();
		iterator iter_end = end();
		while(iter != iter_end) *(iter ++) = val;
		return *this;
	}
public:
	reference operator[](size_type idx) { return _buf[idx]; }
	const_reference operator[](size_type idx) const { return _buf[idx]; }

public:
	reference at(size_type idx)
	{
		if(idx < 0 || idx >= _Length)
			throw std::range_error("out of range of smart array!");
		return _buf[idx];
	}

	const_reference at(size_type idx) const
	{
		if(idx < 0 || idx >= _Length)
			throw std::range_error("out of range of smart array!");
		return _buf[idx];
	}
private:
	_Ty _buf[_Length];
};

template<typename _Tx>
class smart_vector
{
public:
	typedef _Tx value_type;
	typedef value_type* pointer;
	typedef const value_type* const_pointer;
	typedef value_type& reference;
	typedef const value_type& const_reference;

	typedef std::size_t size_type;

	//typedef stdext::checked_array_iterator<array_iterator<value_type>> iterator;
	//typedef stdext::checked_array_iterator<array_iterator<const value_type>> const_iterator;
	typedef array_iterator<value_type> iterator;
	typedef array_iterator<const value_type> const_iterator;

	typedef std::reverse_iterator<iterator> reverse_iterator;
	typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

	typedef smart_vector<_Tx> _Myt;
protected:
	struct block
	{
		block(pointer ptr = NULL, size_type c = 0, bool d = true)
			: values(ptr), count((size_t)c), is_auto_delete(d)
		{}

		~block()
		{
#ifdef _DEBUG
			if((values == NULL && count != 0) ||
				(values != NULL && count == 0))
				throw std::runtime_error("invalidate memory block!");
#endif
			if(values != NULL && is_auto_delete)
			{
				delete []values;
			}
			values = NULL;
			is_auto_delete = false;
			count = 0;
		}

		pointer values;
		size_t count;
		bool is_auto_delete;

		void swap(block& _Other) throw()
		{
			std::swap(values, _Other.values);
			std::swap(count, _Other.count);
			std::swap(is_auto_delete, _Other.is_auto_delete);
		}

		NON_COPYABLE_OBJECT(block)
	};
public:
	smart_vector()
	{}

	smart_vector(size_type _N)
		: buf_(_N != 0 ? (new value_type[_N]) : NULL, _N, true)
	{}

	smart_vector(size_type _N, const_reference _Val)
		: buf_(_N != 0 ? (new value_type[_N]) : NULL, _N, true)
	{
		for(iterator iter = begin(); iter != end(); ++ iter)
			*iter = _Val;
	}

	smart_vector(const smart_vector& _Other)
	{
		if (!_Other.buf_.is_auto_delete)
		{
			buf_.values = _Other.buf_.values;
			buf_.count = _Other.buf_.count;
			buf_.is_auto_delete = _Other.buf_.is_auto_delete;
		}
		else
		{
			buf_.values = _Other.buf_.count != 0 ? (new value_type[_Other.buf_.count]) : NULL;
			buf_.count = _Other.buf_.count;
			buf_.is_auto_delete = _Other.buf_.is_auto_delete;
			std::copy(_Other.begin(), _Other.end(), begin());
		}
	}

	smart_vector(pointer _Buf, std::size_t _N)
		: buf_(_Buf, _N, false)
	{
	}

	smart_vector(const_pointer _Buf, std::size_t _N)
		: buf_(_N != 0 ? (new value_type[_N]) : NULL, _N, true)
	{
		//memcpy(buf_.values, _Buf, size_in_bytes());
		std::copy(_Buf, _Buf + _N, kog::array_begin<1>(buf_.values, buf_.count));
	}

	template<typename _InIt>
	smart_vector(_InIt _First, _InIt _Last)
	{
		std::size_t Len = (std::size_t)std::distance(_First, _Last);
		block tmp(Len != 0 ? (new value_type[Len]) : NULL, Len, true);
		std::copy(_First, _Last, iterator(tmp.values, Len));
		tmp.swap(buf_);
	}

public:
	smart_vector& reset(size_type _N)
	{
		block tmp(_N != 0 ? (new value_type[_N]) : NULL, _N, true);
		tmp.swap(buf_);
		return *this;
	}

	smart_vector& attach(pointer _Ptr, size_type _N, bool _AutoDelete = true)
	{
		block tmp(_Ptr, _N, _AutoDelete);
		tmp.swap(buf_);
		return *this;
	}

	smart_vector& detach()
	{
		block tmp;
		tmp.swap(buf_);
		return *this;
	}

	pointer release()
	{
		block tmp(NULL, 0, false);
		tmp.swap(buf_);
		return tmp.values;
	}

public:
	size_t size() const {return buf_.count;}
public:
	iterator begin() { return iterator(buf_.values, buf_.count); }
	const_iterator begin() const { return const_iterator(buf_.values, buf_.count); }
	iterator end() { return iterator(buf_.values + buf_.count, buf_.count, 0); }
	const_iterator end() const { return const_iterator(buf_.values + buf_.count, buf_.count, 0); }

public:
	reverse_iterator rbegin() { return reverse_iterator(end()); }
	reverse_iterator rend() { return reverse_iterator(begin()); }
	const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
	const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }

public:
	pointer get() { return buf_.values; }
	const_pointer get() const { return buf_.values; }

	reference front()
	{
		assert_validate();
		return buf_.values[0];
	}
	reference back()
	{
		assert_validate();
		return buf_.values[buf_.count-1];
	}

	const_reference front() const
	{
		assert_validate();
		return buf_.values[0];
	}

	const_reference back() const
	{
		assert_validate();
		return buf_.values[buf_.count-1];
	}
public:
	_Myt& operator=(const _Tx& _val)
	{
		iterator iter = begin();
		iterator iter_end = end();
		while(iter != iter_end) *(iter ++) = _val;
		return *this;
	}

	_Myt& operator=(const _Myt& _Other)
	{
		_Myt tmp(_Other);
		tmp.swap(*this);
		return *this;
	}

public:
	size_t size_in_bytes() const {return size() * sizeof(_Tx); }
public:
	reference operator[](size_type idx) { return buf_.values[idx]; }
	const_reference operator[](size_type idx) const { return buf_.values[idx]; }

public:
	reference at(size_type idx)
	{
		if(idx < 0 || idx >= size())
			throw std::range_error("out of range of smart vector!");
		return buf_.values[idx];
	}

	const_reference at(size_type idx) const
	{
		if(idx < 0 || idx >= size())
			throw std::range_error("out of range of smart vector!");
		return buf_.values[idx];
	}
public:
	void swap(_Myt& _Other)
	{
		buf_.swap(_Other.buf_);
	}
protected:
	void assert_validate() const
	{
		if(buf_.values == NULL)
			throw std::out_of_range("empty vector!");
	}
private:

	block buf_;
};

NAMESPACE_END(kog)

#endif
