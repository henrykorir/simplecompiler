// session.h: define session of asm file
//

#ifndef _SESSION_H_SC_
#define _SESSION_H_SC_

#include <basic_types.h>
#include "variable.h"
#include "tuple.h"

NAMESPACE_BEGIN(compile)
NAMESPACE_BEGIN(runtime)

class scope;
class session
{
public:
	session(scope* env, const _Str& name);
	virtual ~session(void);
public:
	MEMBER_VARIABLE_GET(const _Str&, name, name_);
	MEMBER_VARIABLE_GET(scope*, env, env_);
private:
	_Str name_;
	scope* env_;
};

class data_session : public session
{
public:
	data_session(scope* env);
	virtual ~data_session();
public:
	MEMBER_VARIABLE_GET_SET(std::deque<variable*>, variables, varlist_);
public:
	void insert(variable* v);
protected:
    std::deque<variable*> varlist_;
};

class text_session : public session
{
public:
	text_session(scope* env);
	virtual ~text_session();
public:
    MEMBER_VARIABLE_GET_SET(std::deque<tuple*>, tuples, tuples_);
protected:
    std::deque<tuple*> tuples_;
};

class rodata_session : public session
{
public:
	rodata_session(scope* env);
	virtual ~rodata_session();
public:
	MEMBER_VARIABLE_GET_SET(std::deque<variable*>, variables, varlist_);
public:
	void insert(variable* v);
protected:
    std::deque<variable*> varlist_;
};

NAMESPACE_END(runtime)
NAMESPACE_END(compile)

#endif
