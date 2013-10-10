// variable.h: variables
// 

#ifndef _VARIABLE_H_SC_
#define _VARIABLE_H_SC_

#include <macros.h>
#include <shared_ptr.h>
#include <map>
#include <sstream>
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

template <typename _T> struct buildinvalue : public object
{
	buildinvalue(const _T& x) : v(x) {}
	_T v;

	/* overwrite */ virtual _Str to_string() const
	{
		std::ostringstream oss;
		oss<<v;
		return oss.str();
	}
};

typedef buildinvalue<int> int_value;
typedef buildinvalue<tstring> string_value;

class variable : public compile::object
{
public:
    variable()
        : name_("$$$")
        , type_(NULL)
		, env_(NULL)
		, init_(NULL)
    {}

    variable(const _Str& name, const type* t, const scope* env, const value* initValue = NULL)
        : name_(name)
        , type_(t)
        , env_(env)
		, init_(initValue)
    {}

public:
    MEMBER_VARIABLE_GET(_Str, name, name_);
    MEMBER_VARIABLE_GET(const type*, vtype, type_);
    MEMBER_VARIABLE_GET(const scope*, env, env_);
	MEMBER_VARIABLE_GET_SET(void*, more, more_);
	MEMBER_VARIABLE_GET(const value*, initVar, init_);
public:
	// get accesstype of this variable
	int32 access_type() const;
public:
    /* overwrite */ virtual _Str to_string() const
    {
#if 1
		if (name_.find("$ro$[") == 0)
		{ // readonly data
			return name_.substr(name_.find(']') + 1);
		}
#else
#endif
        return name_;
    }
private:
    _Str name_; // name of variable
	const value* init_; // const static value
    const type* type_; // variable's type
    const scope* env_; // scope
	void* more_; // extend information

#ifdef DEBUG_INFO
public:
	const char* Fname;
	uint32 Lcount;
	uint32 Ccount;
#endif
};

class pointer_variable : public variable
{
public:
	pointer_variable();
	pointer_variable(const tstring& name, const scope* env);
public:
	const type* basic_type;
};

class type_variable : public variable
{
public:
	type_variable();
	type_variable(const tstring& name, const scope* env);
public:
	const type* typevar;
};

class complex_variable : public variable
{
public:
	complex_variable();
	complex_variable(const tstring& name, const scope* env);
	//~complex_variable();
public:
	object*& operator[](const tstring& member_name);
protected:
	kog::shared_ptr<std::map<tstring, object*> > members_; 
};

NAMESPACE_END(runtime)
NAMESPACE_END(compile)

#endif
