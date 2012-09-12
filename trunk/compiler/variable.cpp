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