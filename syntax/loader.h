// loader.h: load grammar from xml file
//

#ifndef _LOADER_H_SC_
#define _LOADER_H_SC_

#include <macros.h>
#include <markuputils.h>

NAMESPACE_BEGIN(compile)

class grammar;

NAMESPACE_BEGIN(syntax)
class loader
{
public:
	loader();
	loader(const std::string& fname);
	virtual ~loader();
public:
	void open(const std::string& fname);
	void readgrammar(grammar& aGrammar);
public:
	friend loader& operator>>(loader& Input, grammar& aGrammar);
public:
	void swap(loader& other);
private:
	xml::ifile file_;
};
NAMESPACE_END(syntax)
NAMESPACE_END(compile)
#endif
