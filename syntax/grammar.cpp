#include "grammar.h"

using namespace compile;

tinygrammar::tinygrammar()
: start_symbol_(-1)
, eplison_symbol_(-1)
, ending_symbol_(-1)
{
}
	
tinygrammar::tinygrammar(const tinygrammar& other)
: productions_(other.productions_)
, symbols_(other.symbols().begin(), other.symbols().end())
, start_symbol_(other.start_symbol_)
, eplison_symbol_(other.eplison_symbol_)
, ending_symbol_(other.ending_symbol_)
{
	symbols_.make_index();
}

tinygrammar::~tinygrammar()
{
}

grammar::grammar()
{
}

grammar::~grammar()
{
}

void tinygrammar::swap(tinygrammar& other) throw()
{
	if(this == &other) return;
	symbols_.swap(other.symbols_);
	productions_.swap(other.productions_);
	std::swap(start_symbol_, other.start_symbol_);
	std::swap(eplison_symbol_, other.eplison_symbol_);
	std::swap(ending_symbol_, other.ending_symbol_);
}

void grammar::swap_kernel(tinygrammar& other) throw ()
{
	((tinygrammar*)this)->swap(other);
}

void grammar::swap(grammar& other) throw ()
{
	if(this == &other) return;
	swap_kernel(other);
	smacs_.swap(other.smacs_);
	seperators_.swap(other.seperators_);
	seperators_.swap(other.whitespaces_);
	keywords_.swap(other.keywords_);
	std::swap(dthenu_, other.dthenu_);
}

grammar::grammar(const tinygrammar& atinyG)
	: tinygrammar(atinyG)
{
	symbols_.make_index();
}

