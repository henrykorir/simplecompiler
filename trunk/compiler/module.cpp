#include "module.h"
#include <scerror.h>
#include <debug.h>
#include <logger.h>

using namespace compile;
using namespace compile::runtime;

variable* datamodule::find_here(const _Str& name)
{
    for (std::deque<variable*>::iterator iter = varlist_.begin(); iter != varlist_.end(); ++ iter)
    {
        if ((*iter)->name() == name) return *iter;
    }
    return NULL;
}

bool datamodule::isexist(const _Str& name)
{
    return find(name) != NULL;
}

variable* datamodule::find(const _Str& name)
{
    return find_here(name);
}

variable* datamodule::entry(const variable& var)
{
    if (find_here(var.name()))
    {
        fire("exist variable: " + var.name());
    }

    variable* v_new = new variable(var);
    varlist_.push_back(v_new);
    return v_new;
}

tuple* codemodule::new_tuple(operations::op oper, const object* src1, const object* src2, const object* dst)
{
    kog::assert::is_true(src1 == NULL || dynamic_cast<const variable*>(src1) != NULL);
    kog::assert::is_true(src2 == NULL || dynamic_cast<const variable*>(src2) != NULL);
    kog::assert::is_true(dst == NULL || dynamic_cast<const variable*>(dst) != NULL);
    logstring("new_tuple");
    four_tuple* pt = new four_tuple();
    pt->oper = oper;
    pt->dst = (variable*)(dst == NULL ? NULL : dynamic_cast<const variable*>(dst));
    pt->src1 = (variable*)(src1 == NULL ? NULL : dynamic_cast<const variable*>(src1));
    pt->src2 = (variable*)(src2 == NULL ? NULL : dynamic_cast<const variable*>(src2));
    tuples_.push_back(pt);
    return pt;
}

tuple* codemodule::new_tuple(operations::op oper, const object* src, const object* dst)
{
    return new_tuple(oper, dst, src, dst);
}
