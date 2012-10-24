// compilersyntax.h : grammar class of the compiler
//

#ifndef _COMPILER_SYNTAX_H_SC_
#define _COMPILER_SYNTAX_H_SC_

#include <macros.h>
#include <basic_types.h>
#include <grammar.h>
#include <vector>
#include <deque>
#include <map>
#include <scerror.h>

NAMESPACE_BEGIN(compile);

NAMESPACE_BEGIN(syntax);

struct operator_t
{
	tstring name;
	tstring txt;
};

struct symbolinfo_t
{
	//int32 sid; // id of the symbol
	//symbol basicinfo; // symbol's basic information
	tstring stype; // type of the symbol
};

struct funcinfo_t
{
	//tstring name;
	//tstring type;
	std::vector<tstring> content; // function's content
};

struct keyword_t
{
	tstring keyword;
	int32 sid;
};

struct complex_symbol_t
{
	enum 
	{
		word = 0x01, regex = 0x02, func = 0x04, keyword = 0x08
	};

	int32 basic_sid;
	int32 new_sid;
	tstring content;
	int32 flag; // word or regex or function?
};

NAMESPACE_END(syntax);

class compiler_grammar : public grammar
{
public:
	typedef syntax::operator_t operator_t;
	typedef syntax::funcinfo_t prodinfo_t;
	typedef syntax::funcinfo_t funcinfo_t;
	typedef syntax::complex_symbol_t complex_symbol_t;
public:
	compiler_grammar();
	~compiler_grammar();
public:
	int32 make_new_nonterm_symbol(const tstring& sname, const tstring& stype);
	int32 make_new_term_symbol(const tstring& sname, const tstring& regexstring, const tstring& stype);
	int32 make_new_production(const tstring& left, const std::vector<tstring>& rights, const prodinfo_t& pinfo);
	int32 make_new_keywords(const tstring& keyword, int32 basic_sid);
	int32 make_new_operators(const tstring& oprs, const tstring& name);
	int32 make_new_delimiters(const tstring& delimiters);
	int32 make_new_complex_symbol(const tstring& sname, int32 sbasicid, const tstring& sfunc);
	int32 make_new_function(const tstring& funcname, const funcinfo_t& info);
public:
	//MEMBER_VARIABLE_GET_SET(std::deque<operator_t>, operators, ops_);
	MEMBER_VARIABLE_GET_SET(std::deque<prodinfo_t>, prodmoreinfos, pinfos_);
	MEMBER_VARIABLE_GET_SET(vecsmacs, statemachines, smacs_);
	MEMBER_VARIABLE_GET_SET(veckeywords, keywords, keywords_);
	MEMBER_VARIABLE_GET_SET(std::deque<complex_symbol_t>, complex_symbols, complexsyms_);
	MEMBER_VARIABLE_GET_SET(tstring, whitespaces, whitespaces_);
	MEMBER_VARIABLE_GET_SET(tstring, seperators, seperators_);
	MEMBER_VARIABLE_GET_SET(int32, starts, start_symbol_);
	MEMBER_VARIABLE_GET_SET(int32, eplisons, eplison_symbol_);
	MEMBER_VARIABLE_GET_SET(int32, endings, ending_symbol_);
private:
	int32 make_terminate(const tstring& sname, const tstring& content, const tstring& stype, int32 flag);
	symbol make_symbol(const tstring& name, int32 sid, bool isTerm);
private:
	std::deque<operator_t> ops_;
	std::deque<prodinfo_t> pinfos_;
	std::deque<complex_symbol_t> complexsyms_;
	std::map<tstring, funcinfo_t> funcs_;
};

NAMESPACE_END(compile);

#endif
