#include "variable.h"
#include "scope.h"

using namespace compile;
using namespace compile::runtime;

int32 variable::access_type() const
{
	if (env_ == NULL)
	{
		fire("invalidate scope!");
	}
	return env_->access_type(name_);
}

pointer_variable::pointer_variable()
: basic_type(NULL)
{}

pointer_variable::pointer_variable(const tstring& name, const scope* env)
: variable(name, pointer_variable_type, env)
{
}

type_variable::type_variable()
: typevar(NULL)
{}

type_variable::type_variable(const tstring& name, const scope* env)
: variable(name, type_variable_type, env)
{
}

complex_variable::complex_variable()
: members_(new std::map<tstring, object*>)
{
	more() = members_.get();
}

complex_variable::complex_variable(const tstring& name, const scope* env)
: variable(name, complex_variable_type, env)
, members_(new std::map<tstring, object*>)
{
	more() = members_.get();
}

object*& complex_variable::operator[](const tstring& member_name)
{
	return (*members_)[member_name];
}