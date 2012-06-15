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

tuple* codemodule::new_tuple(const operation* oper, const object* src1, const object* src2, const object* dst)
{
    logstring("new_tuple op(%s), src1(%s), src2(%s), dst(%s)", 
            oper->to_string().c_str(),
            src1 != NULL ? src1->to_string().c_str() : "<null>",
            src2 != NULL ? src2->to_string().c_str() : "<null>",
            dst != NULL ? dst->to_string().c_str() : "<null>");
    four_tuple* pt = new four_tuple();
    pt->oper = oper->oper;
    pt->dst = (variable*)dynamic_cast<const variable*>(dst);
    pt->src1 = (variable*)dynamic_cast<const variable*>(src1);
    pt->src2 = (variable*)dynamic_cast<const variable*>(src2);
    tuples_.push_back(pt);
    return pt;
}

tuple* codemodule::new_tuple(const operation* oper, const object* src, const object* dst)
{
    return new_tuple(oper, dst, src, dst);
}
