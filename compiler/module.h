// module.h: data module, code module
//

#ifndef _MODULE_H_SC_
#define _MODULE_H_SC_

#include <basic_types.h>
#include "variable.h"
#include "tuple.h"
#include "scope.h"
#include <memory>
#include <shared_ptr.h>


NAMESPACE_BEGIN(compile)
NAMESPACE_BEGIN(runtime)

class module
{
public:
	module(const _Str& name);
	module(const _Str& name, scope* scp);
	~module();
public:
	MEMBER_VARIABLE_GET(const _Str&, name, name_);
	MEMBER_VARIABLE_REF(scope*, root_scope, rootScope_.get());
private:
	// root scope(global scope)
	kog::shared_ptr<scope> rootScope_;

	// filename of the module
	_Str name_;
};

NAMESPACE_END(runtime);
NAMESPACE_END(compile)
#endif
