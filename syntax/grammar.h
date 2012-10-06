// grammar.h: language grammar
//

#ifndef _GRAMMAR_H_SC_
#define _GRAMMAR_H_SC_

#include <macros.h>
#include <arrayX.h>
#include <functionalX.h>
#include "production.h"
#include "symbol.h"
#include "automachine.h"

#include <set>

NAMESPACE_BEGIN(compile)

class tinygrammar
{
	friend class syntaxgenerator;
public:
	//typedef std::vector<production> production_holder;
	//typedef kog::smart_vector<production> prodholder_proxy;
	//typedef std::vector<symbol> symbol_holder;
	//typedef kog::smart_vector<symbol> symholder_proxy;
	typedef std::map<tstring, int32> symbol_table;
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
		reset_symbol_string();
		make_index();
	}
	tinygrammar(const tinygrammar& other);
	virtual ~tinygrammar();
public:
	int32 index(const tstring& name) const;
public:
	symholder_proxy symbols() const;
	prodholder_proxy productions() const;
	MEMBER_VARIABLE_GET(int32, starts, start_symbol_);
	MEMBER_VARIABLE_GET(int32, eplisons, eplison_symbol_);
	MEMBER_VARIABLE_GET(int32, endings, ending_symbol_);
	MEMBER_VARIABLE_GET(int32, anys, any_symbol_);
public:
	void swap(tinygrammar& other) throw();
protected:
	void make_index();
private:
	void reset_symbol_string();
protected:
	static std::allocator<tchar> alloc_;
	production_holder productions_; // productions
	symbol_table nameIndexer_; // indexer
	symbol_holder symbols_; // all symbols
	int32 start_symbol_; // start symbol sid
	int32 eplison_symbol_; // eplison symbol sid: -1 not in grammar
	int32 ending_symbol_; // ending symbol sid: -1 no
	int32 any_symbol_; // any(.) symbol's sid: -1 no
};

class grammar : public tinygrammar
{
	friend class syntaxgenerator;
public:
	typedef std::vector<kog::triple<tstring, int32, bool> > vecsmacs; // first: symbol.sid, second: regex string, third: is_direct?
	typedef std::vector<std::pair<tstring, int32> > veckeywords;
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
	MEMBER_VARIABLE_GET/*_SET*/(int32, starts, start_symbol_);
	MEMBER_VARIABLE_GET/*_SET*/(int32, eplisons, eplison_symbol_);
	MEMBER_VARIABLE_GET/*_SET*/(int32, endings, ending_symbol_);
	MEMBER_VARIABLE_GET(const tstring&, whitespaces, whitespaces_);
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
