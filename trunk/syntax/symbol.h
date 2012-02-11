// symbol.h: symbol
//

#ifndef _SYMBOL_H_SC_
#define _SYMBOL_H_SC_

#include <cstring>

#include <macros.h>
#include <basic_types.h>
#include <singleton.h>
#include <arrayX.h>

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

class symholder : public kog::smart_vector<symbol>
{
//	NON_COPYABLE_OBJECT(symholder);
public:
//	enum { nchars = 256, syssid = nchars, usrsid = syssid + 10};
//	enum { epsilon = syssid + 1 };
public:
	symholder()
	{
	}
	
	template<typename _StrIter, typename _SymIter>
	symholder(_StrIter _First_Str, _StrIter _End_Str, _SymIter _First_Sym, _SymIter _End_Sym)
	: kog::smart_vector<symbol>(std::distance(_First_Sym, _End_Sym))
	, symcontentstring_(1 + std::distance(_First_Str, _End_Str))
	{
		memset(get(), 0, size_in_bytes());
		std::copy(_First_Sym, _End_Sym, begin());
		std::copy(_First_Str, _End_Str, symcontentstring_.get());
	}

	template<typename _Iter_Sym>
	symholder(_Iter_Sym _First, _Iter_Sym _End) 
	: kog::smart_vector<symbol>(std::distance(_First, _End))
	{
		int32 nS = 0;
		_Iter_Sym iter = _First;
		while(iter != _End) nS += (iter ++)->Lname + 1;
		symcontentstring_.reset(nS);
		std::copy(_First, _End, begin());
		iter  = _First;
		tchar* p = symcontentstring_.get();
		iterator selfiter = begin();
		while(iter != _End)
		{
			selfiter->name = p;
			std::copy(iter->name, iter->name + iter->Lname, p);
			p[iter->Lname] = '\0';
			p += iter->Lname + 1;
			++ iter;
			++ selfiter;
		}
	}
public:
	int index(const tstring& name) const;
	void make_index();
public:
	void swap(symholder& other)
	{
		if(this == &other) return;
		((kog::smart_vector<symbol>*)this)->swap(other);
		symcontentstring_.swap(other.symcontentstring_);
		nameIndexer_.swap(other.nameIndexer_);
	}
public:
	const tchar* getstringptr() const
	{
		return symcontentstring_.get();
	}
private:
	void updata_sys_symbols();
private:
	kog::smart_vector<tchar> symcontentstring_;
	std::map<const tchar*, const symbol*, stringless> nameIndexer_;
};

NAMESPACE_END(compile)

#endif
