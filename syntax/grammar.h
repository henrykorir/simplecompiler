// grammar.h: language grammar
//

#ifndef _GRAMMAR_H_SC_
#define _GRAMMAR_H_SC_

#include <macros.h>
#include <arrayX.h>
#include "production.h"
#include "symbol.h"
#include "automachine.h"

#include <set>

NAMESPACE_BEGIN(compile)

class tinygrammar
{
	friend class syntaxgenerator;
public:
	typedef kog::smart_vector<production> vecprods;
public:
	tinygrammar();
	template<typename _Iter_Sym, typename _Iter_Prod>
	tinygrammar(_Iter_Sym _First_Sym, _Iter_Sym _Last_Sym, _Iter_Prod _First_Prod, _Iter_Prod _Last_Prod, int32 _Start, int32 _Eplison = -1, int32 _Ending = -1)
	: symbols_(_First_Sym, _Last_Sym)
	, productions_(_First_Prod, _Last_Prod)
	, start_symbol_(_Start)
	, eplison_symbol_(_Eplison)
	, ending_symbol_(_Ending)
	{
		symbols_.make_index();
	}
	tinygrammar(const tinygrammar& other);
	virtual ~tinygrammar();
public:
	MEMBER_VARIABLE_GET_SET(symholder, symbols, symbols_);
	MEMBER_VARIABLE_GET_SET(vecprods, productions, productions_);
	MEMBER_VARIABLE_GET_SET(int32, starts, start_symbol_);
	MEMBER_VARIABLE_GET_SET(int32, eplisons, eplison_symbol_);
	MEMBER_VARIABLE_GET_SET(int32, endings, ending_symbol_);
public:
	void swap(tinygrammar& other) throw();
protected:
	vecprods productions_; // productions
	symholder symbols_; // all symbols
	int32 start_symbol_; // start symbol sid
	int32 eplison_symbol_; // eplison symbol sid: -1 not in grammar
	int32 ending_symbol_; // ending symbol sid: -1 no
};

class grammar : protected tinygrammar
{
	friend class syntaxgenerator;
public:
	typedef kog::smart_vector<std::pair<tstring, int32> > vecsmacs; // first: symbol.sid, second: regex string
	typedef kog::smart_vector<std::pair<tstring, int32> > veckeywords;
public:
	grammar();
	explicit grammar(const tinygrammar& atinyG);
	virtual ~grammar();
public:
	enum symtype
	{
		nont = 0, //non-terminate
		// followings are terminate
		sep = 1, // seperators
		keyword = 2, // keyword
		smac = 3,	// symbol match state machine
		other_sym
	};
public:
	const tinygrammar& gettinyg() const
	{
		return *((const tinygrammar*)this);
	}
public:
	void swap(grammar& other) throw ();
	void swap_kernel(tinygrammar& tg) throw ();
protected:
	vecsmacs smacs_; // state machines
	veckeywords keywords_;
	tstring whitespaces_;	// whitespace: space tab or some other charactors
	tstring seperators_;	// seperators: split word
protected:
	bool dthenu_; // symbol need be defined before use?
};

NAMESPACE_END(compile)
#endif
