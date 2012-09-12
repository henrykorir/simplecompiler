#include "session.h"

using namespace compile;
using namespace compile::runtime;

session::session(scope* env, const _Str& name)
: name_(name)
, env_(env)
{
}

session::~session(void)
{
}

data_session::data_session(scope* env)
: session(env, typeid(data_session).name())
{
}

data_session::~data_session()
{
}

void data_session::insert(variable* v)
{
	varlist_.push_back(v);
}

text_session::text_session(scope* env)
: session(env, typeid(text_session).name())
{
}

text_session::~text_session()
{
}

rodata_session::rodata_session(scope* env)
: session(env, typeid(rodata_session).name())
{
}

rodata_session::~rodata_session()
{
}

void rodata_session::insert(variable* v)
{
	varlist_.push_back(v);
}

