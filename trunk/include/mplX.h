// mplX.h :
//

#ifndef _MPLX_H_HUXL_
#define _MPLX_H_HUXL_

#include <macros.h>

NAMESPACE_BEGIN(kog)
NAMESPACE_BEGIN(mpl)

template<typename _T>
struct is_const_t 
{
	const static bool value = false;
};

template<typename _T>
struct is_const_t<const _T>
{
	const static bool value = true;
};

template<typename _T>
struct add_const
{
	typedef const _T type;
	typedef _T nonconst_type;
};

template<typename _T>
struct add_const<const _T>
{
	typedef const _T type;
	typedef _T nonconst_type;
};

template<bool, typename _TrueType, typename _FalseType>
struct choos_t {};

template<typename _TrueType, typename _FalseType>
struct choos_t<true, _TrueType, _FalseType>
{
	typedef _TrueType type;
};

template<typename _TrueType, typename _FalseType>
struct choos_t<false, _TrueType, _FalseType>
{
	typedef _FalseType type;
};

template<typename _Tx>
struct remove_const
{
	typedef _Tx type;
	typedef const _Tx const_type;
};

template<typename _Tx>
struct remove_const<const _Tx>
{
	typedef _Tx type;
	typedef const _Tx const_type;
};

template<typename _T>
struct fire_error
{
	const static int value = sizeof(_T);
};

template<typename _Tx>
struct const_reference
{
	typedef const _Tx& type;
};

template<typename _Tx>
struct const_reference<const _Tx&>
{
	typedef const _Tx& type;
};

template<typename _Tx>
struct const_reference<const _Tx>
{
	typedef const _Tx& type;
};

template<typename _Tx>
struct const_reference<_Tx&>
{
	typedef const _Tx& type;
};

//template<typename _T>
//struct fire_error<false

template<typename _Tx>
struct parse_class_value_type
{
	typedef _Tx class_type;
	typedef _Tx value_type;
};

template<typename _Tc, typename _Ty>
struct parse_class_value_type<_Ty _Tc::*>
{
	typedef _Tc class_type;
	typedef _Ty value_type;
};

NAMESPACE_END(mpl)
NAMESPACE_END(kog)

#endif
