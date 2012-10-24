// cplcompiler.h : c pragram language compiler
//

#ifndef _COMPILER_H_CPL_
#define _COMPILER_H_CPL_

#include <compiler.h>
#include <compilersyntax.h>
#include <lalr1machine.h>
#include <symbolmachine.h>
#include <buckethash.h>

NAMESPACE_BEGIN(compile);
NAMESPACE_BEGIN(cplc);

struct keyword_hashid : public std::unary_function<tstring, int32>
{
	int32 operator()(const tstring& keywords) const;
};

class cplcompiler
{
public:
	typedef bool (*complex_symbol_convert)(automachine::machine_meta* meta);
	typedef std::vector<complex_symbol_convert> veccsconver;
	typedef void (*loadgrammar)(tinygrammar& g, lalr1machine& m, symbol_machine& s, kog::smart_vector<veccsconver>& symconvertor);
public:
	cplcompiler();
	virtual ~cplcompiler();
public:
	// load compiler syntax ...
	void setup(loadgrammar loader);
public:
	void init();
	void build(tistream& cfile);
private:
	std::auto_ptr<compiler_grammar> g_;
	std::auto_ptr<lalr1machine> m_;
	std::auto_ptr<symbol_machine> s_;
	//kog::buckethash<tstring, int32, keyword_hashid> keywords_;
	//loadgrammar loader_;
	kog::smart_vector<veccsconver> symconvertor_; // is sid(index of array item) need convert?
};

NAMESPACE_END(cplc);
NAMESPACE_END(compile);

#endif
