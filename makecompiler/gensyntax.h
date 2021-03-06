// gensyntax.h : grammar -> syntax cpp files
//

#ifndef _GENSYNTAX_H_SC_
#define _GENSYNTAX_H_SC_

#include <macros.h>
#include <basic_types.h>
#include <ostream>
#include <istream>

#include "grammar.h"
#include "compilersyntax.h"

NAMESPACE_BEGIN(compile)

class syntaxgenerator
{
public:
	syntaxgenerator();
	virtual ~syntaxgenerator();
public:
	void operator()(const grammar* aGrammar, const tstring& outputfolder, const tstring& grammarname);
private:
	void print_hfile();
	void print_cppfile();
	void print_rpfile();
private:
	void print_compiler();
	void print_construct();
	void print_grammar();
	void print_symbols();
	void print_statemachines();
	void print_includes();
	void print_separators();
	void print_keywords();
	void print_whitespaces();
	void print_keyword_convert_funcs();
	void print_complexsymbols();
	void print_printablechars();
	void print_machine(std::ostream& os, const compile::automachine& mac);
    void print_productions();
	void print_class();
private:
	void regex_str_to_machine(const std::string& regexstr, automachine& m, bool is_directly);
	void refine_machine(automachine& m) const;
	bool is_token_keyword(const tchar* name) const;
public:
	const grammar* syntax_;
	tstring gname_;
	tstring ofolder_;
	std::ostream* cppstream_;
private:
	static const tstring compiler_hfile_fmt;
	static const tstring compiler_cppfile_fmt;
	static const tstring reduce_functions_fmt;
};

class function_parser
{
public:
    function_parser(std::ostream& os)
        : cppstream_(&os)
    {}

public:
    function_parser& operator()(const _Str& comment, const _Str& func, const _Str& name);
	function_parser& operator()(const _Str& comment, const compiler_grammar::prodinfo_t& infos, const _Str& name);
private:

private:
    std::ostream* cppstream_;
};

NAMESPACE_END(compile)
#endif

