// variable.h: variables
// 

#ifndef _VARIABLE_H_SC_
#define _VARIABLE_H_SC_

#include <macros.h>
#include "type.h"
#include "object.h"
#include "value.h"

NAMESPACE_BEGIN(compile)
NAMESPACE_BEGIN(runtime)

class scope;

class variable : public compile::object
{
public:
    variable()
        : name_("$$$")
        , pos_(-1)
        , type_(NULL)
    {}

    variable(const _Str& name, const compile::type* t, const scope* env, uint32 add = -1)
        : name_(name)
        , pos_(add)
        , type_(t)
        , env_(env)
    {}

    variable(const _Str& name, const compile::type* t, const scope* env, value* initial)
        : name_(name)
        , initv_(initial)
        , type_(t)
        , env_(env)
    {}
public:
    MEMBER_VARIABLE_GET(_Str, name, name_);
    MEMBER_VARIABLE_GET_SET(uint32, address, pos_);
    MEMBER_VARIABLE_GET_SET(value*, initvalue, initv_);
    MEMBER_VARIABLE_GET(const compile::type*, vtype, type_);
    MEMBER_VARIABLE_GET(const scope*, env, env_);
private:
    _Str name_; // name of variable
    union {
        uint32 pos_; // address in memory
        value* initv_; // initial value
    };
    const compile::type* type_; // variable's type
    const scope* env_;

#ifdef DEBUG_INFO
public:
	const char* Fname;
	uint32 Lcount;
	uint32 Ccount;
#endif
};

NAMESPACE_END(runtime)
NAMESPACE_END(compile)

#endif
