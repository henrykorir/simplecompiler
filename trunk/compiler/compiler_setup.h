// compiler_setup.h : c pragram language compiler
//

#ifndef _COMPILER_H_CPL_
#define _COMPILER_H_CPL_

#include <compilersyntax.h>
#include "lalr1machine.h"
#include <symbolmachine.h>
#include <buckethash.h>
#include "compiler.h"

NAMESPACE_BEGIN(compile);

struct keyword_hashid : public std::unary_function<tstring, int32>
{
	int32 operator()(const tstring& keywords) const;
};

class compiler_setup
{
	friend class ::compiler;
public:
	typedef bool (*complex_symbol_convert)(compile::automachine::machine_meta* meta);
	typedef std::vector<complex_symbol_convert> veccsconver;
	//typedef void (*loadgrammar)(tinygrammar& g, lalr1machine& m, symbol_machine& s, kog::smart_vector<veccsconver>& symconvertor);
public:
	compiler_setup();
	virtual ~compiler_setup();
public:
	// load compiler syntax ...
	void setup();
protected:
	virtual void init_separators(kog::smart_vector<sc::int32>& separators, kog::tree<int32>& sepsid) = 0;
	virtual void init_complex_symbols(kog::smart_vector<veccsconver>& convertors) = 0;
	virtual void init_keywords(kog::buckethash<std::string, int32, string_2_int>& keywords) = 0;
	virtual void init_printablechars(kog::smart_vector<sc::int32>& printablechars) = 0;
	virtual void init_machines(std::map<std::string, compile::doc::machine>& machines) = 0;
	virtual void init_syntax_machine(compile::lalr1machine& lrm) = 0;
	virtual void init_grammar(compile::tinygrammar& tig) = 0;
	virtual void init_production_functions(kog::smart_vector<compile::ifunction*>& pfuncs) = 0;
	virtual void init_whitespaces(bool& is_skipwhitespace, tstring& whitespaces) = 0;
protected:
	std::map<std::string, compile::doc::machine> machines_;
	std::auto_ptr<compiler_grammar> g_;
	std::auto_ptr<lalr1machine> m_;
	std::auto_ptr<symbol_machine> s_;
	//kog::buckethash<tstring, int32, keyword_hashid> keywords_;
	//loadgrammar loader_;
	kog::smart_vector<veccsconver> symconvertor_; // is sid(index of array item) need convert?
};

NAMESPACE_END(compile);

#endif
