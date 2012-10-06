// compilermake.h: 
//

#ifndef _COMPILERMAKE_H_SC_
#define _COMPILERMAKE_H_SC_

#include <macros.h>
#include <basic_types.h>
#include <ostream>
#include <deque>
#include <map>

#include "compilersyntax.h"

NAMESPACE_BEGIN(compile);

class makecompiler
{
public:
	typedef std::deque<_Str> deqfiles;
	typedef std::map<tstring, tstring> strmap;
	typedef std::pair<tstring, tstring> strpair;
private:
	struct fileposition
	{
		fileposition(const tchar* f)
			: filename(f), iLine(0), iCol(0)
		{}

		const tchar* filename;
		size_t iLine;
		size_t iCol;
	};
public:
	struct quick_replacer
	{
		tchar meta;
		quick_replacer* firstchild;
		quick_replacer* nextbrother;
		int32 depth;
	};
public:
	makecompiler(const _Str& mainfile);
	~makecompiler();
public:
	void build_compiler(const _Str& cppfile);
private:
	// parse a given file
	size_t parse_file(const std::string& syntaxfile, compiler_grammar& cg);
private:
	// read seprators
	size_t parse_seprators(std::istream& is, compiler_grammar& cg);
	// read opereators
	size_t parse_operators(std::istream& is, compiler_grammar& cg);
	// read productions
	size_t parse_production(std::istream& is, compiler_grammar& cg, const std::string& prod);
	// read includes
	size_t parse_includes(std::istream& is, compiler_grammar& cg);
	// read non-terminate symbol
	size_t parse_nonterms(std::istream& is, compiler_grammar& cg);
	// read terminate symbol
	size_t parse_terms(std::istream& is, compiler_grammar& cg);
	// read keywords
	size_t parse_keywords(std::istream& is, compiler_grammar& cg);
private:
	bool get_nextline(std::istream& is, std::string& tmp, size_t& flines) const;

	void _replace(tstring& tmp) const;
	void _new_replacer(const tstring& pattern, const tstring& replacer);
	int32 _need_replace(const tchar* p) const;
protected:
	// get error string of curfile_
	tstring errorpos() const;
private:
	_Str mainfile_;
	deqfiles allfiles_;
	fileposition curfile_;

	// replace all first with second
	strmap replacer_;
	quick_replacer starter_;
};

NAMESPACE_END(compile);
#endif
