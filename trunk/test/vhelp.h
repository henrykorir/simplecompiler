// vhelp.h: 
//

#ifndef _VHELP_H_TEST_
#define _VHELP_H_TEST_

#include <deque>
#include <basic_types.h>
#include <symbol.h>
#include <production.h>

struct symindex
{
	typedef compile::int32 int32;
	typedef compile::tstring tstring;

	symindex(int32 u)
		: idx(u)
	{
	}

	symindex(const tstring& name)
		: idx(symbols()->index(name))
	{
	}

	symindex(const compile::tchar* name)
		: idx(symbols()->index(name))
	{
	}

	operator int32() const
	{
		return idx;
	}

	static const compile::symholder*& symbols()
	{
		static const compile::symholder* __local_p = NULL;
		return __local_p;
	}

	int32 idx;
};

// v functions
class V : public kog::smart_vector<compile::int32>
{
	typedef compile::int32 int32;
	typedef compile::tstring tstring;
	typedef kog::smart_vector<int32> v;
#define IDX const symindex&
public:
	V() {}
	V(IDX _0) : v(1)
	{
		(*this)[0] = _0.idx;
	}

	V(IDX _0, IDX _1) : v(2)
	{
		(*this)[0] = _0.idx;
		(*this)[1] = _1.idx;
	}

	V(IDX _0, IDX _1, IDX _2) : v(3)
	{
		(*this)[0] = _0.idx;
		(*this)[1] = _1.idx;
		(*this)[2] = _2.idx;
	}

	V(IDX _0, IDX _1, IDX _2, IDX _3) : v(4)
	{
		(*this)[0] = _0.idx;	(*this)[1] = _1.idx;
		(*this)[2] = _2.idx;	(*this)[3] = _3.idx;
	}

	V(IDX _0, IDX _1, IDX _2, IDX _3, IDX _4) : v(5)
	{
		(*this)[0] = _0.idx;	(*this)[1] = _1.idx;
		(*this)[2] = _2.idx;	(*this)[3] = _3.idx;
		(*this)[4] = _4.idx;
	}

	V(IDX _0, IDX _1, IDX _2, IDX _3, IDX _4, IDX _5) : v(6)
	{
		(*this)[0] = _0.idx;	(*this)[1] = _1.idx;
		(*this)[2] = _2.idx;	(*this)[3] = _3.idx;
		(*this)[4] = _4.idx;	(*this)[5] = _5.idx;
	}

	V(IDX _0, IDX _1, IDX _2, IDX _3, IDX _4, IDX _5, IDX _6) : v(7)
	{
		(*this)[0] = _0.idx;	(*this)[1] = _1.idx;
		(*this)[2] = _2.idx;	(*this)[3] = _3.idx;
		(*this)[4] = _4.idx;	(*this)[5] = _5.idx;
		(*this)[6] = _6.idx;
	}

	V(IDX _0, IDX _1, IDX _2, IDX _3, IDX _4, IDX _5, IDX _6, IDX _7) : v(8)
	{
		(*this)[0] = _0.idx;	(*this)[1] = _1.idx;
		(*this)[2] = _2.idx;	(*this)[3] = _3.idx;
		(*this)[4] = _4.idx;	(*this)[5] = _5.idx;
		(*this)[6] = _6.idx;	(*this)[7] = _7.idx;
	}
	
	V(IDX _0, IDX _1, IDX _2, IDX _3, IDX _4, IDX _5, IDX _6,
			IDX _7, IDX _8, IDX _9, IDX _10, IDX _11, IDX _12) : v(13)
	{
		(*this)[0] = _0.idx;	(*this)[1] = _1.idx;
		(*this)[2] = _2.idx;	(*this)[3] = _3.idx;
		(*this)[4] = _4.idx;	(*this)[5] = _5.idx;
		(*this)[6] = _6.idx;	(*this)[7] = _7.idx;
		(*this)[8] = _8.idx;	(*this)[9] = _9.idx;
		(*this)[10] = _10.idx;	(*this)[11] = _11.idx;
		(*this)[12] = _12.idx;
	}
	
	static const compile::symholder*& symbols()
	{
		return symindex::symbols();
	}

};

class Asymbol : public compile::symbol
{
	typedef compile::int32 int32;
	typedef compile::tstring tstring;
	typedef compile::tchar tchar;
public:
	Asymbol(const tstring& sname, int32 ist, const tstring& regex_exp = "")
	{
		static int32 uid = 0;
		sid = uid++;
		this->ist = ist != 0;
		name = (const tchar*)(stringBuf.size()); // using idx here
		stringBuf.insert(stringBuf.end(), sname.begin(), sname.end());
		stringBuf.push_back(tchar('\0'));
		Lname = sname.size();
		Lfuncs = 0;
		funcs = NULL;

		switch(ist)
		{
		case compile::grammar::keyword: //keywords
		//case compile::grammar::sep:
			{
				keywords.push_back(std::make_pair(regex_exp, sid));
				break;
			}
		case compile::grammar::smac: // make state_machine
			{
				smacs.push_back(std::make_pair(regex_exp, sid));
				break;
			}
		}
	}
public:
	static std::deque<tchar> stringBuf;
	static std::deque<std::pair<tstring, int32> > keywords;
	static std::deque<std::pair<tstring, int32> > smacs;
};

class Aproduction : public compile::production
{
	typedef compile::int32 int32;
	typedef compile::tstring tstring;
public:
	Aproduction(int32 L, const V& v)
		: compile::production(*V::symbols(), L, v.get(), v.size())
	{
	}
};

#endif
