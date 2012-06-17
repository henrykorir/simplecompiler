#include "scope.h"
#include <cstdarg>
#include <memory>
#include <stringX.h>
#include <logger.h>

using namespace compile;
using namespace compile::runtime;

scope::scope()
: noname_idx_(0)
{
    staticdata_ = kog::shared_ptr<datamodule>(new datamodule());
    stackdata_ = kog::shared_ptr<datamodule>(new datamodule());
    code_ = kog::shared_ptr<codemodule>(new codemodule());
}

scope::scope(scope* parent)
: noname_idx_(0)
, parent_(parent)
, staticdata_(parent->staticdata_)
{
    stackdata_ = kog::shared_ptr<datamodule>(new datamodule());
    code_ = kog::shared_ptr<codemodule>(new codemodule());
}

scope::~scope()
{
}

variable* scope::find(const _Str& name)
{
    variable* p = stackdata_->find(name);
    if (p == NULL && staticdata_.get() != NULL)
    {
        p = staticdata_->find(name);
    }
    return p ? p : (parent_ ? parent_->find(name) : NULL);
}

variable* scope::entry_variable(const _Str& vname, const type* vtype, int32 vscope)
{ 
    logstring("entry_variable, name %s", vname.c_str());
    variable v(vname, vtype, this, -1);
    variable* pv = NULL;
    switch(vscope)
    {
        case varscope::stack:
            pv = stackdata_->entry(v);
            code_->new_tuple(new operation(operations::newv), pv, NULL, NULL);
            break;
        case varscope::global:
            pv = staticdata_->entry(v);
            break;
    }
    return pv;
}
    
variable* scope::entry_function(const _Str& fname, const type* result_type, int32 nparams, ...)
{
    va_list argptr;
	va_start(argptr, nparams);
    const type* p1 = va_arg(argptr, const type*);
    const function_type* ft = typesystem::instance().new_func_type(nparams, &p1, result_type);
    va_end(argptr);
    
    logstring("entry_function, name %s", fname.c_str());

    variable v(fname, ft, this, -1);
    return staticdata_->entry(v);
}

variable* scope::entry_value(const _Str& content, const type* canTypes[], int _C)
{
    logstring("entry_value, content %s", content.c_str());
    std::auto_ptr<variable> var;
    value* v = new value();
    _Str name = stringX::format("noname_var%d", noname_idx_ ++);
    if (content.find('.') != _Str::npos)
    {
        v->initv = (byte*)new double(stringX::tovalue<double>(content));
        v->size = sizeof(double);
        var.reset(new variable(name, typesystem::instance().float_type(), this, v));
    }
    else
    {
        v->initv = (byte*)new int(stringX::tovalue<int>(content));
        v->size = sizeof(int);
        var.reset(new variable(name, typesystem::instance().int_type(), this, v));
    }

    return staticdata_->entry(*var);
}

tuple* scope::entry_tuple(const operation* oper, const object* src1, const object* src2, const object* dst)
{
    if (code_.get() != NULL)
    {
        return code_->new_tuple(oper, src1, src2, dst);
    }
    return NULL;
}