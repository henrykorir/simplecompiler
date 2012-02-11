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

template<typename _Ty, size_t _Row, size_t _Col>
struct SmartMatrix
{
public:
	typedef std::size_t size_type;
	typedef typename mpl::remove_const<_Ty>::type value_type;
	typedef array_iterator<value_type> iterator;
	typedef array_iterator<const value_type> const_iterator;
	typedef std::reverse_iterator<iterator> reverse_iterator;
	typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
	typedef SmartMatrix<_Ty, _Row, _Col> _Myt;
	typedef value_type& reference;
	typedef const value_type& const_reference;
	typedef _Ty* pointer;
	typedef const _Ty* const_pointer;

	typedef pointer RowVector;
	typedef const_pointer ConstRowVector;

public:
	const static size_t Row = _Row;
	const static size_t Column = _Col;
	const static size_t Length = _Row * _Col;
public:
	SmartMatrix()
	{}

	// input from a value
	SmartMatrix(const _Ty& val)
	{
		memset(_values, val, sizeof(_values));
	}

	// input from any iterator range
	template<typename _IterIn>
	SmartMatrix(_IterIn _First, _IterIn _Last)
	{
		std::copy(_First, _Last, begin());
	}

public:
	_Ty* ptr() { return _values[0]; }
	const _Ty* ptr() const { return _values[0]; }

public:
	iterator begin() { return iterator(_values[0], Length); }
	iterator end() { return iterator(_values[0] + Length, Length, 0); }
	const_iterator begin() const { return const_iterator(_values[0], Length); }
	const_iterator end() const { return const_iterator(_values[0] + Length, Length, 0); }
public:
	size_t row() const { return _Row; }
	size_t col() const { return _Col; }
	size_t size() const {return Length; }
public:
	// operator[]
	RowVector operator[](size_type idx) {return _values[idx];}
	ConstRowVector operator[](size_type idx) const{return _values[idx];}

public:
	reference operator()(size_type r, size_type c) { return _values[r][c]; }
	const_reference operator()(size_type r, size_type c) const { return _values[r][c]; }
private:
	_Ty _values[_Row][_Col];
};


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
		: buf_(_Other.size() != 0 ? (new value_type[_Other.size()]) : NULL,
			_Other.size(), true)
	{
		std::copy(_Other.begin(), _Other.end(), begin());
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

template<typename _Tx>
class smart_matrix
{
public:
};

//template<typename _Tx, size_t _M, size_t _N, size_t _K>
//struct multiplies
//	//: public std::binary_function<
//	//	SmartMatrix<_Tx, _M, _N>, SmartMatrix<_Tx, _N, _K>,
//	//	SmartMatrix<_Tx, _M, _K> >
//{
//	SmartMatrix<_Tx, _M, _K> operator()(const SmartMatrix<_Tx, _M, _N>& A, const SmartMatrix<_Tx, _N, _K>& B) const
//	{
//		SmartMatrix<_Tx, _M, _K> C;
//		for(int m = 0; m < _M; ++ m)
//		{
//			for(int k = 0; k < _K; ++ k)
//			{
//				C[m][k] = 0;
//				for(int n = 0; n < _N; ++ n)
//				{
//					C[m][k] += A[m][n] * B[n][k];
//				}
//			}
//		}
//		return C;
//	}
//};
//
//template<typename _Tx, size_t _M, size_t _N>
//struct multiplies
//	//: public std::binary_function<
//{
//	SmartArray<_Tx, _M> operator()(const SmartMatrix<_Tx, _M, _N>& A, const SmartArray<_Tx, _N>& b) const
//	{
//		SmartArray<_Tx, _M> t;
//		for(int m = 0; m < _M; ++ m)
//		{
//			t[m] = 0;
//			for(int n = 0; n < _N; ++ n)
//			{
//				t[m] += A[m][n] * b[n];
//			}
//		}
//		return t;
//	}
//
//	SmartArray<_Tx, _N> operator()(const SmartArray<_Tx, _M>& b, const SmartMatrix<_Tx, _M, _N>& A) const
//	{
//		SmartArray<_Tx, _N> t;
//		for(int n = 0; n < _N; ++ n)
//		{
//			t[n] = 0;
//			for(int m = 0; m < _M; ++ m)
//			{
//				t[n] += A[m][n] * b[m];
//			}
//		}
//		return t;
//	}
//};
//
//// dot
//template<typename _Tx, size_t _N>
//struct multiplies
//	//: public std::binary_function<
//	//	SmartArray<_Tx, _N>, SmartArray<_Tx, _N>, SmartArray<_Tx, _N> >
//{
//	_Tx operator()(const SmartArray<_Tx, _N>& a, const SmartArray<_Tx, _N>& b) const
//	{
//		_Tx t = 0;
//		for(int n = 0; n < _N; ++ n)
//		{
//			t += a[n] * b[n];
//		}
//		return t;
//	}
//
//	SmartArray<_Tx, _N> operator()(const SmartArray<_Tx, _N>& v, const _Tx& a) const
//	{
//		SmartArray<_Tx, _N> t;
//		std::transform(v.begin(), v.end(), t.begin(),
//			std::bind2nd(std::multiplies<_Tx>(), a));
//		return t;
//	}
//
//	SmartArray<_Tx, _N> operator()(const _Tx& a, const SmartArray<_Tx, _N>& v) const
//	{
//		SmartArray<_Tx, _N> t;
//		std::transform(v.begin(), v.end(), t.begin(),
//			std::bind2nd(std::multiplies<_Tx>(), a));
//		return t;
//	}
//}; 
//
//template<typename _Tx, size_t _N>
//struct plus
//	: public std::binary_function<SmartArray<_Tx, _N>, SmartArray<_Tx, _N>, SmartArray<_Tx, _N> >
//{
//	SmartArray<_Tx, _N> operator()(const SmartArray<_Tx, _N>& v1, const SmartArray<_Tx, _N>& v2) const
//	{
//		SmartArray<_Tx, _N> t;
//		std::transform(v1.begin(), v1.end(), v2.begin(), t.begin(),
//			std::plus<_Tx>());
//		return t;
//	}
//};
//
//template<typename _Tx, size_t _M, size_t _N>
//struct plus
//	: public std::binary_function<SmartMatrix<_Tx, _M, _N>, SmartMatrix<_Tx, _M, _N>, SmartMatrix<_Tx, _M, _N> >
//{
//	SmartMatrix<_Tx, _M, _N> operator()(const SmartMatrix<_Tx, _M, _N>& m1, const SmartMatrix<_Tx, _M, _N>& m1) const
//	{
//		SmartArray<_Tx, _M, _N> t;
//		std::transform(m1.begin(), m1.end(), m2.begin(), t.begin(),
//			std::plus<_Tx>());
//		return t;
//	}
//};
//
//template<typename _Tx, size_t _N>
//struct minus
//	: public std::binary_function<SmartArray<_Tx, _N>, SmartArray<_Tx, _N>, SmartArray<_Tx, _N> >
//{
//	SmartArray<_Tx, _N> operator()(const SmartArray<_Tx, _N>& v1, const SmartArray<_Tx, _N>& v2) const
//	{
//		SmartArray<_Tx, _N> t;
//		std::transform(v1.begin(), v1.end(), v2.begin(), t.begin(),
//			std::minus<_Tx>());
//		return t;
//	}
//};
//
//template<typename _Tx, size_t _M, size_t _N>
//struct minus
//	: public std::binary_function<SmartMatrix<_Tx, _M, _N>, SmartMatrix<_Tx, _M, _N>, SmartMatrix<_Tx, _M, _N> >
//{
//	SmartMatrix<_Tx, _M, _N> operator()(const SmartMatrix<_Tx, _M, _N>& m1, const SmartMatrix<_Tx, _M, _N>& m1) const
//	{
//		SmartArray<_Tx, _M, _N> t;
//		std::transform(m1.begin(), m1.end(), m2.begin(), t.begin(),
//			std::minus<_Tx>());
//		return t;
//	}
//};

NAMESPACE_END(kog)

#endif
