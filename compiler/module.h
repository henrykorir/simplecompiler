// module.h: data module, code module
//

#ifndef _MODULE_H_SC_
#define _MODULE_H_SC_

#include <basic_types.h>
#include "variable.h"
#include "tuple.h"

NAMESPACE_BEGIN(compile)
NAMESPACE_BEGIN(runtime)

class module
{
};

class datamodule : virtual public module
{
public:
	bool isexist(const _Str& name);
    virtual variable* find(const _Str& name);
	virtual variable* entry(const variable& v);
protected:
    variable* find_here(const _Str& name);
protected:
    std::deque<variable*> varlist_;
};

class codemodule : virtual public module
{
public:
    // dst = src1 op src2
    tuple* new_tuple(const operation* oper, const object* src1, const object* src2, const object* dst);

    // dst = dst op src
    tuple* new_tuple(const operation* oper, const object* src, const object* dst);
protected:
    std::deque<tuple*> tuples_;
};

NAMESPACE_END(runtime);
NAMESPACE_END(compile)
#endif
