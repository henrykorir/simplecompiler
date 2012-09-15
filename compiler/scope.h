// scope.h: 
//

#ifndef _SCOPE_H_SC_
#define _SCOPE_H_SC_

#include <macros.h>
#include <vector>
#include <shared_ptr.h>
#include <map>

#include "session.h"

NAMESPACE_BEGIN(compile)
NAMESPACE_BEGIN(runtime)

namespace varscope
{
    enum scope_defines
    {
        vparam,
        vstack,
        vheap,
        vglobal,
        vmodule
    };

	enum access_type
	{
		_public,
		_protected,
		_private
	};

	enum scope_type
	{
		sunknown,
		sglobal,
		sfunction,
		sstruct,
		sunoin,
		senum
	};
}

class scope : public object
{
public:
	typedef std::map<_Str, session*> session_map;
	typedef std::map<_Str, variable*> symboltable;
	typedef std::map<_Str, int32> access_info_map;
public:
    scope();
    scope(scope* parent, int32 stype, bool isInherit = true);
    virtual ~scope() = 0;
public:
	// and a new const value object
    variable* entry_value(const _Str& content, const type* canTypes[], int _N);

	// regist a new function
    variable* entry_function(int ass_type, const _Str& fname, function_type* ftype);

	// regist a new variable
	// ass_type is access type of the variable(public/protected/private or more)
    variable* entry_variable(const _Str& vname, const type* vtype, int ass_type, int32 vscope = varscope::vstack);

	// add new op
	tuple* entry_tuple(const operation* oper, const object* src1, const object* src2, const object* dst);

	// find variable/function using given name
	variable* find(const _Str& name);

	// get access type of given variable
	int32 access_type(const _Str& str) const;
protected:
	variable* find_here(const _Str& name) const;
public:
	// get parent scope
    MEMBER_VARIABLE_GET_SET(scope*, parent, parent_);
	// sizeof current scope(object)
	// if the scope is a function, return statck size
	// else if is class/struct, return size of its member variables
	// else if is global scope, return size of all static data
	MEMBER_VARIABLE_GET_SET(size_t, bytes, bytes_);

	MEMBER_VARIABLE_REF(std::deque<scope*>&, children, children_);

	MEMBER_VARIABLE_GET(const session_map&, sessions, sessions_);

	MEMBER_VARIABLE_GET_SET(_Str, name, name_);
private:
	// parent scope
    scope* parent_;

	std::deque<scope*> children_;

	// using list
    std::vector<scope*> usingList_;

	std::deque<scope*> refList_;

	// temp variable's name index
    int32 noname_idx_;

	size_t bytes_;

	// scope type
	int32 stype_;
private:
	symboltable reg_table_;
	access_info_map access_;
protected:
    _Str name_;
//    scope* definedscope_;

	// session map
	session_map sessions_;
};

class slicescope : public scope
{
public:
	slicescope(scope* parent = NULL)
		: scope(parent, varscope::sglobal, true)
	{}

	/* overwrite */ virtual ~slicescope()
	{}

};

class funcscope : public scope
{
public:
	funcscope(scope* parent, function_type* ftype)
		: scope(parent, varscope::sfunction, true)
		, funtype_(ftype)
	{
		if (ftype != NULL && ftype->return_type != NULL)
		{
			retVar_ = new variable("##retValue##", ftype->return_type, this);
		}
	}

	/* overwrite */ virtual ~funcscope()
	{}
public:
	MEMBER_VARIABLE_GET(function_type*, ftype, funtype_);
	MEMBER_VARIABLE_GET(variable*, return_var, retVar_);
private:
	// return variable
	// if return type of the function is 'void', retVar_ == NULL
	// else retVar_->type() == typeof(return type of the function)
	variable* retVar_;

	// function type of the function
	function_type* funtype_;
};

NAMESPACE_END(runtime)
NAMESPACE_END(compile)

#endif
