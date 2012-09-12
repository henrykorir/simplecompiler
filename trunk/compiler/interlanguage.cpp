#include "interlanguage.h"
#include <functional>
#include <algorithm>
#include <logger.h>
#include <fstream>
#include <scerror.h>
#include <typeinfo>

#include "extract.h"
#include "lalr1machine.h"
#include "asmgenerate.h"

using namespace sc;
using namespace compile;
using namespace compile::doc;
using namespace compile::runtime;

interlanguage::interlanguage()
: main_scope_(new slicescope())
{
	main_scope_->name() = "global_scope";
    current_scope_ = main_scope_;
    all_scopes_.push_back(current_scope_);
	logstring("current scope(%s)", main_scope_->name().c_str());
}

interlanguage::~interlanguage()
{ 
	foreach (scope*& s, all_scopes_.begin(), all_scopes_.end())
    {
		//delete s;
		s = NULL;
    }
    all_scopes_.clear();
}

scope* interlanguage::push_scope(scope* s)
{
	logstring("push scope(%s)", s->name().c_str());
	s->parent() = current_scope_;
	current_scope_->children().push_back(s);
    all_scopes_.push_back(s);
    return current_scope_ = s;
}

scope* interlanguage::into_scope(runtime::scope* s)
{
    current_scope_ = s;
    return s;
}

scope* interlanguage::otof_scope()
{
    if (current_scope_ == NULL)
    {
        fire("root scope!");
    }
	logstring("out of scope(%s)", current_scope_->name().c_str());
    current_scope_ = current_scope_->parent();
	logstring("current scope is %s", current_scope_ == NULL ? "<null>" : current_scope_->name().c_str());
	return current_scope_;
}

scope* interlanguage::current_scope() const
{
    return current_scope_;
}

void interlanguage::generate(const std::string& finput_name, const std::string& fotput_name)
{
	module m(finput_name, main_scope_);
	std::ofstream ofile(fotput_name.c_str());
	if (!ofile) fire("can't open file: " + fotput_name);
	asmgenerate asmg(ofile);
	//asmg.print(&m);
	//
	foreach (scope* ps, all_scopes_.begin(), all_scopes_.end())
	{
		asmg.print(ps);
	}
	ofile.close();
}
