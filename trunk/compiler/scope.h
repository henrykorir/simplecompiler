// scope.h: 
//

#ifndef _SCOPE_H_SC_
#define _SCOPE_H_SC_

#include <macros.h>
#include <vector>
#include <shared_ptr.h>

#include "module.h"

NAMESPACE_BEGIN(compile)
NAMESPACE_BEGIN(runtime)

namespace varscope
{
    enum scope_defines
    {
        stack,
        heap,
        global,
        module
    };
}

class scope : public datamodule, public codemodule
{
public:
    scope();
    scope(scope* parent);
    ~scope();
public:
    variable* entry_value(const _Str& content, const type* canTypes[], int _N);
    variable* entry_function(const _Str& fname, const type* result_type, int32 nparams, ...);
    variable* entry_variable(const _Str& vname, const type* vtype, int32 vscope = varscope::stack);
	/* overwrite */ virtual variable* find(const _Str& name);
public:
    MEMBER_VARIABLE_GET(scope*, parent, parent_);
private:
    scope* parent_;
    std::vector<scope*> usingList_;
    kog::shared_ptr<datamodule> staticdata_;
    int32 noname_idx_;
};

NAMESPACE_END(runtime)
NAMESPACE_END(compile)

#endif
