// production.h: production (asym -> asym, ...)
//

#ifndef _PRODUCTION_H_SC_
#define _PRODUCTION_H_SC_

#include <macros.h>
#include <basic_types.h>

#include "symbol.h"

NAMESPACE_BEGIN(compile)

class production
{
	friend class tinygrammar;
public:
	typedef kog::smart_vector<int32> right_array;
public:
	production();
	production(const symholder_proxy& syms);
	production(int32 L, const int32* Rs, int32 N);
	production(const symholder_proxy& syms, int32 L, const int32* Rs, int32 N);
public:
	MEMBER_VARIABLE_GET_SET(int32, left, L_);
	MEMBER_VARIABLE_GET(int32, right_size, Rs_.size());
	MEMBER_VARIABLE_GET(const right_array&, right, Rs_);
	MEMBER_VARIABLE_GET_SET_INDEXER(int32, std::size_t, Rs_);
    MEMBER_VARIABLE_GET_SET(_Str, func, func_);
public:
	void reset_right(const int32* Rs, int N);

	// convert production to string
	tstring to_string() const;
public:
	void swap(production& other);
private:
	symholder_proxy holder_;
	int32 L_;
	right_array Rs_;
    _Str func_;
};

template<typename _Less>
struct pleft_cmp : public std::binary_function<production, production, bool>
{
	bool operator()(const production& p1, const production& p2) const
	{
		_Less _L;
		return _L(p1.left(), p2.left());
	}
};

template<typename _Less>
struct pfirstright_cmp : public std::binary_function<production, production, bool>
{
	bool operator()(const production& p1, const production& p2) const
	{
		_Less _L;
		return _L(p1.right().at(0), p2.right().at(0));
	}
};

template<typename _Less, typename _LorR> struct pright_cmp;
template<typename _Less>
struct pright_cmp<_Less, std::logical_or<bool> > : public std::binary_function<production, production, bool>
{
	bool operator()(const production& p1, const production& p2) const
	{
		_Less _L;
		int32 _N = std::min(p1.right_size(), p2.right_size());
		for(int32 i = 0; i < _N; ++ i) if(_L(p1.right()[i], p2.right()[i])) return true;
		return _L(p1.right_size(), p2.right_size());
	}
};

template<typename _Less>
struct pright_cmp<_Less, std::logical_and<bool> > : public std::binary_function<production, production, bool>
{
	bool operator()(const production& p1, const production& p2) const
	{
		_Less _L;
		int32 _N = std::min(p1.right_size(), p2.right_size());
		for(int32 i = 0; i < _N; ++ i) if(!_L(p1.right()[i], p2.right()[i])) return false;
		return _L(p1.right_size(), p2.right_size());
	}
};

template<typename _Less, typename _LorR> struct p_cmp;
template<typename _Less>
struct p_cmp<_Less, std::logical_or<bool> > : public std::binary_function<production, production, bool>
{
	bool operator()(const production& p1, const production& p2) const
	{
		_Less _L;
		pright_cmp<_Less, std::logical_or<bool> > _RL;
		if(_L(p1.left(), p2.left())) return true;
		else return _RL(p1, p2);
	}
};

template<typename _Less>
struct p_cmp<_Less, std::logical_and<bool> > : public std::binary_function<production, production, bool>
{
	bool operator()(const production& p1, const production& p2) const
	{
		_Less _L;
		pright_cmp<_Less, std::logical_and<bool> > _RL;
		if(!_L(p1.left(), p2.left())) return false;
		else return _RL(p1, p2);
	}
};

struct prod_less : public std::binary_function<production, production, bool>
{
	bool operator()(const production& p1, const production& p2) const
	{
		if(p1.left() != p2.left()) return p1.left() < p2.left();

		const int32 _N = std::min(p1.right_size(), p2.right_size());
		int32 i = 0;
		for(; i < _N && p1.right()[i] == p2.right()[i]; ++ i);
		if (i == _N) return p1.right_size() < p2.right_size();
		return p1.right()[i] < p2.right()[i];
	}
};

typedef pleft_cmp<std::less<int32> > pleft_less;
typedef pfirstright_cmp<std::less<int32> > pfirstright_less;
//typedef p_cmp<std::less<int32>, std::logical_or<bool> > p_less;
typedef p_cmp<std::equal_to<int32>, std::logical_and<bool> > p_equal;

typedef std::vector<production> production_holder;
typedef kog::smart_vector<production> prodholder_proxy;

NAMESPACE_END(compile)
#endif
