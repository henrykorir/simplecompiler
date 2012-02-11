/*#include "loader.h"

using namespace compile;
using namespace compile::syntax;

#define fire(msg)\
	throw std::runtime_error("[syntax::loader] "msg)
loader::loader()
{
}

loader::loader(const std::string& fname)
: file_(fname)
{
}

void loader::open(const std::string& fname)
{
	loader tmp(fname);
	swap(tmp);
}
void loader::swap(loader& other)
{
	file_.swap(other.file_);
}

grammar& loader::readgrammar(grammar& agrammar)
{
	xml::node& root = file_.cnode();
	if(root->name() != "ProgramLanguage") fire("invalidate root!");

	tstring language = root["Name"];
	tstring fext = root["FileExtentsion"];

	return agrammar;
}

loader& operator>>(loader& Input, grammar& agrammar)
{
	Input.readgrammar(agrammar);
	return Input;
}*/

