#include "module.h"
#include <scerror.h>
#include <debug.h>
#include <logger.h>

using namespace compile;
using namespace compile::runtime;

module::module(const _Str& name)
: rootScope_(new slicescope(NULL))
, name_(name)
{
}

module::module(const _Str& name, scope* scp)
: rootScope_(scp)
, name_(name)
{
	if (scp == NULL)
		fire("invalidate scope!");
}

module::~module()
{
}

