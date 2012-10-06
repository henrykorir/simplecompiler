// symbol.h: symbol
//

#ifndef _SYMBOL_H_SC_
#define _SYMBOL_H_SC_

#include <cstring>

#include <macros.h>
#include <basic_types.h>
#include <singleton.h>
#include <arrayX.h>
#include <vector>
#include <list>

#include <map>
#include <algorithm>

NAMESPACE_BEGIN(compile)

struct symfunc
{
	tstring name;
	typedef void* (*function)(void*);
	function func;
};

struct symbol
{
	int32 sid; // symbol id: used to index symbol
	const tchar* name;
	symfunc* funcs;
	int16 Lname;
	int8 Lfuncs;
	int8 ist;
};

struct stringless : public std::binary_function<const tchar*, const tchar*, bool>
{
	bool operator()(const tchar* str1, const tchar* str2) const
	{
		while(*str1 && *str1 == *str2) ++ str1, ++ str2;
		return  *str1 < *str2;
	}
};

template<typename _CmpFunc>
struct symbolname_Cmp : public std::binary_function<symbol, symbol, bool>
{
	bool operator()(const symbol& s1, const symbol& s2) const
	{
		return _Cmp(s1.name, s2.name);
	}

	_CmpFunc _Cmp;
};

typedef symbolname_Cmp<stringless> symbolname_Less;

typedef std::vector<symbol> symbol_holder;
typedef kog::smart_vector<symbol> symholder_proxy;

NAMESPACE_END(compile)

#endif
