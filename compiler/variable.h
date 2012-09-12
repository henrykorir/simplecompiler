// variable.h: variables
// 

#ifndef _VARIABLE_H_SC_
#define _VARIABLE_H_SC_

#include <macros.h>
#include "type.h"
#include "object.h"

NAMESPACE_BEGIN(compile)
NAMESPACE_BEGIN(runtime)

class scope;

struct value : public object
{
    union {
        uint32 address;
        byte* initv;
    };
    uint32 size;
};

class variable : public compile::object
{
public:
    variable()
        : name_("$$$")
        , type_(NULL)
		, env_(NULL)
		, init_(NULL)
    {}

    variable(const _Str& name, const compile::type* t, const scope* env, const value* initValue = NULL)
        : name_(name)
        , type_(t)
        , env_(env)
		, init_(initValue)
    {}

public:
    MEMBER_VARIABLE_GET(_Str, name, name_);
    MEMBER_VARIABLE_GET(const compile::type*, vtype, type_);
    MEMBER_VARIABLE_GET(const scope*, env, env_);
	MEMBER_VARIABLE_GET_SET(void*, more, more_);
	MEMBER_VARIABLE_GET(const value*, initVar, init_);
public:
	// get accesstype of this variable
	int32 access_type() const;
public:
    /* overwrite */ virtual _Str to_string() const
    {
        return name_;
    }
private:
    _Str name_; // name of variable
	const value* init_; // const static value
    const compile::type* type_; // variable's type
    const scope* env_; // scope
	void* more_; // extend information

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
