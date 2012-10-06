// hxl_functional.h :
//

#ifndef _FUNCTIONAL_H_HUXL_
#define _FUNCTIONAL_H_HUXL_

#include <numeric>
#include <functional>
#include <algorithm>
#include <vector>
#include <macros.h>
#include <mplX.h>
//#include <arrayX.h>
//#include <exceptionX.h>

NAMESPACE_BEGIN(kog)

enum TriSate
{
	Yes = 1,
	No = -1,
	UnKnown = 0
};

template<typename _Arg,
	typename _Less_Equal = std::less_equal<_Arg> >
class is_between_t : public std::unary_function<bool, _Arg>
{
public:
	is_between_t(const _Arg& _min, const _Arg& _max)
		: _minValue(_min), _maxValue(_max) {}
public:
	bool operator()(const _Arg& val) const
	{
		_Less_Equal _lessEqual;
		return _lessEqual(_minValue, val) && _lessEqual(val, _maxValue);
	}
private:
	const _Arg& _minValue;
	const _Arg& _maxValue;
};

template<typename Value_Type>
inline bool IsBetween(const Value_Type& min_value, const Value_Type& max_value, const Value_Type& value)
{
	is_between_t<Value_Type> _isBetween(min_value, max_value);
	return _isBetween(value);
}

// generate a sequence [_From, _To) = (_From, _From + _Diff, _From + 2*_Diff, ..., _To - _Diff)
// param@ _From squence start from
// param@ _Diff
// param@ _To
// param@ _First
template<typename _T, typename _Iter>
inline _Iter genLinear(_T _From, _T _Diff, _T _To, _Iter _First)
{
	while(_From < _To)
	{
		*(_First ++) = _From;
		_From += _Diff;
	}
	return _First;
}


template<typename _T, typename _Iter>
inline _Iter genLinearC(_T _From, _T _To, size_t _Count, _Iter _First)
{
	if(_Count == 0) return _First;
	else if(_Count == 1)
	{
		*(_First ++) = _From;
		return _First;
	}
	_T diff = (_To - _From) / (_Count - 1);
	while(_Count --)
	{
		*(_First ++) = _From;
		_From += diff;
	}
	return _First;
}


template<typename Ptr_Type>
inline void safe_delete(Ptr_Type& ptr)
{
	if(ptr != NULL)
	{
		delete ptr;
	}
	ptr = NULL;
}

template<typename Ptr_Type>
inline void safe_delete_vec(Ptr_Type& ptr)
{
	if(ptr != NULL) delete [] ptr;
	ptr = NULL;
}

template<typename _InIt, typename _Ty, typename _Fn2>
inline void accumulate(_InIt _First1, _InIt _Last, _InIt _First2, _Ty _Val, _Fn2 _Func)
{
	for(; _First1 != _Last; ++ _First1, ++ _First2)
	{
		_Val = _Func(_Val, *_First1, *_First2);
	}
	return _Val;
}

template<typename _InIt, typename _Ty, typename _Fn2, typename _Op>
inline void accumulate(_InIt _First1, _InIt _Last, _InIt _First2, _Ty _Val, _Fn2 _Func, _Op _Opt)
{
	for(; _First1 != _Last; ++ _First1, ++ _First2)
	{
		_Val = _Func(_Val, _Opt(*_First1, *_First2));
	}
	return _Val;
}

// three yuan funciton y = f(x, y, z)
template<typename _Arg1, typename _Arg2, typename _Arg3, typename _Result>
struct triple_function
{
	typedef _Arg1 first_argument_type;
	typedef _Arg2 second_argument_type;
	typedef _Arg3 third_argument_type;
	typedef _Result result_type;
};

// make a function(only take one argument) to a functional object
template<typename _Arg, typename _Result>
struct unary_function_adptor_t : public std::unary_function<_Arg, _Result>
{
public:
	typedef _Result (*_FunType)(_Arg);

	unary_function_adptor_t(_FunType pfun)
		: _pFun(pfun)
	{
		if(NULL == _pFun)
			throw std::exception("invalid function pointer!");
	}

	_Result operator()(_Arg arg) const
	{
		return _pFun(arg);
	}

private:
	_FunType _pFun;
};

// make a function(take two argument) to a functional object
template<typename _Arg1, typename _Arg2, typename _Result>
struct binary_function_adptor_t : public std::binary_function<_Arg1, _Arg2, _Result>
{
public:
	typedef _Result (*_FunType)(_Arg1, _Arg2);

	binary_function_adptor_t(_FunType pfun)
		: _pFun(pfun)
	{
		if(NULL == _pFun)
			throw std::exception("invalid function pointer!");
	}

	_Result operator()(_Arg1 arg1, _Arg2 arg2) const
	{
		return _pFun(arg1, arg2);
	}

private:
	_FunType _pFun;
};

template<typename _Arg1, typename _Arg2, typename _Arg3, typename _Result>
struct triple_function_adptor_t : public triple_function<_Arg1, _Arg2, _Arg3, _Result>
{
public:
	typedef _Result (*_FunType)(_Arg1, _Arg2, _Arg3);

	triple_function_adptor_t(_FunType pfun)
		: _pFun(pfun)
	{
		if(NULL == _pFun)
			throw std::exception("invalid function pointer!");
	}

	_Result operator()(_Arg1 arg1, _Arg2 arg2, _Arg3 arg3) const
	{
		return _pFun(arg1, arg2, arg3);
	}

private:
	_FunType _pFun;
};

template<typename _Arg1, typename _Arg2, typename _Arg3, typename _Result>
inline triple_function_adptor_t<_Arg1, _Arg2, _Arg3, _Result> triple_function_adptor(_Result (*_Func)(_Arg1, _Arg2, _Arg3))
{
	return triple_function_adptor_t<_Arg1, _Arg2, _Arg3, _Result>(_Func);
}

template<typename _Arg1, typename _Arg2, typename _Result>
inline binary_function_adptor_t<_Arg1, _Arg2, _Result> binary_function_adptor(_Result (*_Func)(_Arg1, _Arg2))
{
	return binary_function_adptor_t<_Arg1, _Arg2, _Result>(_Func);
}

template<typename _Arg, typename _Result>
inline unary_function_adptor_t<_Arg, _Result> unary_function_adptor(_Result (*_Func)(_Arg))
{
	return unary_function_adptor_t<_Arg, _Result>(_Func);
}

// reference object
template<typename T>
class ref_obj
{
public:
	typedef T& ref_type;
	typedef const T& const_ref_type;
public:
	ref_obj(T& obj)
		: _ref_obj(obj)
	{}

public:
	operator ref_type()
	{
		return _ref_obj;
	}

	operator const_ref_type() const
	{
		return _ref_obj;
	}

public:
	T* operator->()
	{
		return &_ref_obj;
	}

private:
	T& _ref_obj;
};

template<typename T>
inline ref_obj<T> ref(T& obj)
{
	return ref_obj<T>(obj);
}

template<typename _Tx>
struct depointer_t : public std::unary_function<_Tx*, _Tx>
{
	_Tx& operator()(_Tx* p) const
	{
		return *p;
	}
};

template<typename _Tx>
struct depointer_t<const _Tx> : public std::unary_function<const _Tx*, const _Tx>
{
	const _Tx& operator()(const _Tx* p) const
	{
		return *p;
	}
};

template<typename _Tx>
depointer_t<_Tx> depointer(_Tx*)
{
	depointer_t<_Tx> v;
	return v;
}

template<typename _Oper, typename _Func1, typename _Func2>
class apply
{
public:
	typedef typename _Oper::result_type result_type;
	typedef typename _Oper::argument_type argument_type;
public:
	apply(){}
	apply(const _Oper& op, const _Func1& func1, const _Func2& func2)
		: _op(op), _func1(func1), _func2(func2)
	{}
public:
	result_type operator()(const argument_type& arg) const
	{
		return _op(_func1(arg), _func2(arg));
	}

private:
	_Oper _op;
	_Func1 _func1;
	_Func2 _func2;
};

template<typename _Arg>
class assign_t : public std::binary_function<_Arg, _Arg, _Arg>
{
public:
	_Arg& operator()(_Arg& _dstVal, const _Arg& _srcVal) const
	{
		return _dstVal = _srcVal;
	}
};

// convert _Tx to _Ty
template<typename _Tx, typename _Ty>
struct explicit_convert_t : public std::unary_function<_Tx, _Ty>
{
	_Ty operator()(const _Tx& _x) const
	{
		return (_Ty)_x;
	}
};

template<typename _Ty, typename _Tx>
explicit_convert_t<_Tx, _Ty> explicit_convert(const _Tx&)
{
	return explicit_convert<_Tx, _Ty>();
}

template<unsigned long long _BinaryValue>
struct binary
{
	const static unsigned int value = binary<_BinaryValue / 10>::value * 2 + _BinaryValue % 10;
};

template<>
struct binary<0>
{
	const static unsigned int value = 0;
};

// TEMPLATE STRUCT triple
template<typename _Ty1,
	typename _Ty2 = _Ty1,
	typename _Ty3 = _Ty2>
struct triple
{
public:
	typedef triple<_Ty1, _Ty2, _Ty3> _Myt;
	typedef _Ty1 first_type;
	typedef _Ty2 second_type;
	typedef _Ty3 third_type;

public:
	triple()
		: first(_Ty1()), second(_Ty2()), third(_Ty3())
		{	// construct from defaults
		}

	triple(const _Ty1& _Val1, const _Ty2& _Val2, const _Ty3& _Val3)
		: first(_Val1), second(_Val2), third(_Val3)
		{	// construct from specified values
		}

	template<class _Other1,
		class _Other2,
		class _Other3>
		triple(const triple<_Other1, _Other2, _Other3>& _Right)
		: first(_Right.first), second(_Right.second), third(_Right.third)
		{	// construct from compatible pair
		}

	void swap(_Myt& _Right)
		{	// exchange contents with _Right
		std::swap(first, _Right.first);
		std::swap(second, _Right.second);
		std::swap(third, _Right.third);
		}

	_Ty1 first;	// the first stored value
	_Ty2 second;	// the second stored value
	_Ty3 third; // the third stored value
};

template<typename _Ty1, typename _Ty2, typename _Ty3>
inline triple<_Ty1, _Ty2, _Ty3> make_triple(_Ty1 _Val1, _Ty2 _Val2, _Ty3 _Val3)
{
	return triple<_Ty1, _Ty2, _Ty3>(_Val1, _Val2, _Val3);
}

template<typename _Tc, typename _Ty>
struct mem_value_t : public std::unary_function<_Tc, _Ty>
{
	typedef _Ty value_type;
	typedef _Ty& reference_type;
	typedef const _Ty& const_reference_type;
	typedef _Tc class_type;
	typedef mem_value_t<_Tc, _Ty> _Myt;

	typedef value_type class_type::* mem_value_type;

public:
	mem_value_t(mem_value_type _p)
		: _p_value(_p)
	{}

public:
	reference_type operator()(class_type& _class) const
	{
		return _class.*_p_value;
	}

	const_reference_type operator()(const class_type& _class) const
	{
		return _class.*_p_value;
	}
protected:
	mem_value_type _p_value;
};

template<typename _Tx>
mem_value_t<typename mpl::parse_class_value_type<_Tx>::class_type,
	typename mpl::parse_class_value_type<_Tx>::value_type> mem_value(_Tx _mp)
{
	return mem_value_t<typename mpl::parse_class_value_type<_Tx>::class_type,
		typename mpl::parse_class_value_type<_Tx>::value_type>(_mp);
}

template<typename _Arg>
struct NonOp : public std::unary_function<_Arg, _Arg>
{
	_Arg& operator()(_Arg& _arg) const
	{
		return _arg;
	}

	const _Arg& operator()(const _Arg& _arg) const
	{
		return _arg;
	}
};

template<typename _Fx, typename _Fy>
struct composite_function_t :
	public std::unary_function<typename _Fx::argument_type, typename _Fy::result_type>
{
	// f := x -> _Fy(_Fx(x))
	typedef typename _Fx::argument_type argument_type;
	typedef typename _Fy::result_type result_type;

	typedef composite_function_t<_Fx, _Fy> _Myt;

	composite_function_t() {}
	composite_function_t(const _Fx& fx, const _Fy& fy)
		: _fx(fx), _fy(fy){}

	result_type operator()(argument_type& _arg) const
	{
		return _fy(_fx(_arg));
	}

	result_type operator()(const argument_type& _arg) const
	{
		return _fy(_fx(_arg));
	}
protected:
	_Fx _fx;
	_Fy _fy;
};

template<typename _Fx, typename _Fy, typename _Fz>
struct composite_function2_t :
	public std::binary_function<typename _Fx::argument_type, typename _Fy::argument_type, typename _Fz::result_type>
{
	// f := (x, y) -> _Fz(_Fx(x), _Fy(y))
	typedef typename _Fx::argument_type first_argument_type;
	typedef typename _Fy::argument_type second_argument_type;
	typedef typename _Fz::result_type result_type;

	typedef composite_function2_t<_Fx, _Fy, _Fz> _Myt;

	composite_function2_t() {}
	composite_function2_t(const _Fx& fx, const _Fy& fy, const _Fz& fz)
		: _fx(fx), _fy(fy), _fz(fz) {}

	result_type operator()(first_argument_type& _arg1, second_argument_type& _arg2) const
	{
		return _fz(_fx(_arg1), _fy(_arg2));
	}

	result_type operator()(const first_argument_type& _arg1, const second_argument_type& _arg2) const
	{
		return _fz(_fx(_arg1), _fy(_arg2));
	}
protected:
	_Fx _fx;
	_Fy _fy;
	_Fz _fz;
};

// new function is f: x -> fy(fx(x))
template<typename _Fx, typename _Fy>
composite_function_t<_Fx, _Fy> composite_function(const _Fx& fx, const _Fy& fy)
{
	return composite_function_t<_Fx, _Fy>(fx, fy);
}

template<typename _Fx, typename _Fy, typename _Fz>
composite_function2_t<_Fx, _Fy, _Fz> composite_function(const _Fx& fx, const _Fy& fy, const _Fz& fz)
{
	return composite_function2_t<_Fx, _Fy, _Fz>(fx, fy, fz);
}

template<typename _Tx, typename _Fun>
struct pointer_binary_function : public std::binary_function<_Tx*, _Tx*, typename _Fun::result_type>
{
	typename _Fun::result_type operator()(_Tx* _Arg1, _Tx* _Arg2) const
	{
		return _Fun()(*_Arg1, *_Arg2);
	}
};

template<typename _Tx>
struct get_ptr_t : public std::unary_function<_Tx, _Tx*>
{
	_Tx* operator()(_Tx& _Arg) const
	{
		return &_Arg;
	}
	
	const _Tx* operator()(const _Tx& _Arg) const
	{
		return &_Arg;
	}
};

template<typename _Iter>
struct range_t : protected std::pair<_Iter, _Iter>
{
	typedef typename mpl::add_const<_Iter>::value_type iterator;
	typedef typename mpl::add_const<_Iter>::const_value_type const_iterator;
	typedef std::pair<_Iter, _Iter> _Mybase;
	//typedef typename _Iter iterator;

	typedef std::pair<_Iter, _Iter> _Base;

	range_t(iterator first, iterator last)
		: _Base(first, last)
	{}

	iterator begin() const
	{
		return _Mybase::first;
	}

	iterator end() const
	{
		return _Mybase::second;
	}
};

template<typename _It>
range_t<_It> range(_It _First, _It _Last)
{
	return range_t<_It>(_First, _Last);
}

template<typename _InIt>
struct dereference_t :
	public std::unary_function<_InIt, typename std::iterator_traits<_InIt>::value_type>
{
	typedef typename std::iterator_traits<_InIt>::reference reference;

	reference operator()(_InIt _Iter) const
	{
		return *_Iter;
	}
};

template<typename _InIt>
std::vector<size_t> index_sort(_InIt _First, _InIt _Last)
{
	return index_sort(_First, _Last, std::less<typename std::iterator_traits<_InIt>::value_type>());
}

template<typename _InIt, typename _Pr>
std::vector<size_t> index_sort(_InIt _First, _InIt _Last, _Pr _pr)
{
	const size_t N = std::distance(_First, _Last);
	typedef std::pair<_InIt, size_t> _Pair;
	std::vector<_Pair > _Buf(N);
	_InIt _Iter = _First;
	size_t _i = 0, _j = 0, _k = 0;
	while(_Iter != _Last)
	{
		_Buf[_i] = std::make_pair(_Iter ++, _i);
		++ _i;
	}

	std::sort(_Buf.begin(), _Buf.end(),
		kog::composite_function(
		kog::composite_function(kog::mem_value(&_Pair::first),
				kog::dereference_t<_InIt>()),
			kog::composite_function(kog::mem_value(&_Pair::first),
				kog::dereference_t<_InIt>()),
			_pr));
	std::vector<size_t> _Idx(N);
	std::transform(_Buf.begin(), _Buf.end(), _Idx.begin(),
		kog::mem_value(&_Pair::second));
	typename std::iterator_traits<_InIt>::value_type _TempValue;
	//std::swap(_TempValue, *_First);

	for(_i = 0; _i != N; ++ _i)
	{
		if(_i == _Buf[_i].second) continue;
		_TempValue = *_Buf[_i].first;
		_j = _i;
		while(_i != _Buf[_j].second)
		{
			_k = _Buf[_j].second;
			*(_Buf[_j].first) = *(_Buf[_k].first);
			_Buf[_j].second = _j;
			_j = _k;
		}
		_Buf[_j].second = _j;
		*(_Buf[_j].first) = _TempValue;
	}
	return _Idx;
}

template<typename _Tx, typename _Less = std::less<_Tx> >
struct pointer_less : public std::binary_function<const _Tx*, const _Tx*, bool>
{
	bool operator()(const _Tx* v1, const _Tx* v2) const
	{
		_Less less;
		return less(*v1, *v2);
	}
};

template<typename _Tx, typename _Equal = std::equal_to<_Tx> >
struct pointer_equal : public std::binary_function<const _Tx*, const _Tx*, bool>
{
	bool operator()(const _Tx* v1, const _Tx* v2) const
	{
		_Equal eq;
		return eq(*v1, *v2);
	}
};

// compaire two pairs p1 < p2 ?: std::pair<_Tx, _Ty>
template<typename _Tx, typename _Ty, 
	typename _LessX = std::less<_Tx>, 
	typename _LessY = std::less<_Ty> >
struct pair_less : 
	public std::binary_function<
		const std::pair<_Tx, _Ty>, 
		const std::pair<_Tx, _Ty>, bool>
{
	typedef std::pair<_Tx, _Ty> pair_type;
	bool operator()(const pair_type& p1, const pair_type& p2) const
	{
		_LessX lx;
		_LessY ly;
		return lx(p1.first, p2.first) ||
			(!lx(p2.first, p1.first) && ly(p1.second, p2.second));
	}
};

// compaire two pairs p1 == p2 ? : std::pair<_Tx, _Ty>
template<typename _Tx, typename _Ty, 
	typename _EqualX = std::equal_to<_Tx>, 
	typename _EqualY = std::equal_to<_Ty> >
struct pair_equal : 
	public std::binary_function<
		const std::pair<_Tx, _Ty>, 
		const std::pair<_Tx, _Ty>, bool>
{
	typedef std::pair<_Tx, _Ty> pair_type;
	bool operator()(const pair_type& p1, const pair_type& p2) const
	{
		_EqualX eqx;
		_EqualY eqy;
		return eqx(p1.first, p2.first) && eqy(p1.second, p2.second);
	}
};

//template<typename _PairType,
//	typename _LessX = std::less<typename _PairType::first_type>,
//	typename _LessY = std::less<typename _PairType::second_type> >
//struct pair_less<typename _PairType::first_type, typename _PairType::second_type, _LessX, _LessY>;

// help class
template<size_t _Iter_size>
struct __range_foreach
{
	// end used for make sure iter type is same
	template<typename _Iter>
	__range_foreach(_Iter cur, _Iter end) 
	{
		memcpy(xxx, &cur, _Iter_size);
		//new (xxx)_Iter(cur);
		first = true;
	}

	template<typename _Iter>
	bool hasNext(_Iter _End) const
	{
		return *((_Iter*)xxx) != _End;
	}

	// param is used for return type
	template<typename _Iter>
	_Iter moveNext(_Iter) 
	{
		return (*((_Iter*)xxx))++;
	}

	operator bool()
	{
		bool b = first;
		first = false;
		return b;
	}

	char xxx[_Iter_size];
	bool first;
};

#ifdef foreach
#error 'exist foreach'
#else
#define foreach(_Value, _First, _End) \
	for (kog::__range_foreach<sizeof(_First)> __r_1__(_First, _End); __r_1__; ) \
		for (int __c_1__ = 1, __b_1__ = 1; __c_1__ && __r_1__.hasNext(_End); (__c_1__ = (__b_1__ == 0) ? 1 : 0), (__b_1__ = 1)) \
			for (_Value = *__r_1__.moveNext(_End); __c_1__ --; -- __b_1__)
#endif
NAMESPACE_END(kog)
#endif
