#include "grammar.h"
#include <scerror.h>
#include <functionalX.h>
#include <memory>

using namespace compile;

tinygrammar::tinygrammar()
: start_symbol_(-1)
, eplison_symbol_(-1)
, ending_symbol_(-1)
{
}
	
tinygrammar::tinygrammar(const tinygrammar& other)
: productions_(other.productions_)
, symbols_(other.symbols_)
, start_symbol_(other.start_symbol_)
, eplison_symbol_(other.eplison_symbol_)
, ending_symbol_(other.ending_symbol_)
{
	reset_symbol_string();
	// alloc new strings
	make_index();
}

std::allocator<tchar> tinygrammar::alloc_;

void tinygrammar::reset_symbol_string()
{
	// update symbol string
	foreach (symbol& s, symbols_.begin(), symbols_.end())
	{
		if (s.name == NULL) 
		{
			s.name = alloc_.allocate(1);
			*(tchar*)s.name = '\0';
		}
		else
		{
			size_t size_in_bytes = sizeof(tchar) * s.Lname + 1;
			std::auto_ptr<tchar> buffer(alloc_.allocate(s.Lname + 1));
			memcpy(buffer.get(), s.name, size_in_bytes);
			s.name = buffer.release();
		}
	}

	// update production symbol_holder ref
	foreach (production& p, productions_.begin(), productions_.end())
	{
		p.holder_ = symbols();
	}
}

tinygrammar::~tinygrammar()
{
	// delete symbols' name string
	foreach (symbol& s, symbols_.begin(), symbols_.end())
	{
		if (s.name) 
		{
			alloc_.deallocate((tchar*)s.name, s.Lname + 1);
			s.name = NULL;
		}
	}
	symbols_.clear();
	productions_.clear();
}

symholder_proxy tinygrammar::symbols() const
{
	symholder_proxy proxy;
	if (!symbols_.empty())
	{
		proxy.attach((symbol*)&symbols_[0], symbols_.size(), false);
	}
	return proxy;
}

prodholder_proxy tinygrammar::productions() const
{
	prodholder_proxy proxy;
	if (!productions_.empty())
	{
		proxy.attach((production*)&productions_[0], productions_.size(), false);
	}
	return proxy;
}

void tinygrammar::make_index()
{
	nameIndexer_.clear();
	foreach (symbol& s, symbols_.begin(), symbols_.end())
	{
		//nameIndexer_[iter->name] = &(*iter);
		nameIndexer_[s.name] = s.sid;
	}
}

int32 tinygrammar::index(const tstring& name) const
{
	if (nameIndexer_.size() != symbols_.size())
		fire("index was not updated");
	symbol_table::const_iterator iter = nameIndexer_.find(name);
	if (iter == nameIndexer_.end()) return -1;
	return iter->second;
}

grammar::grammar()
: skip_whitespaces_(true)
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
	nameIndexer_.swap(other.nameIndexer_);
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
}

