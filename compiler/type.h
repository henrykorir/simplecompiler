// type.h: types
//

#ifndef _TYPES_H_SC_
#define _TYPES_H_SC_

#include "object.h"
#include <arrayX.h>
#include <stringXF.h>
#include <singleton.h>
#include <deque>
#include <basic_types.h>

NAMESPACE_BEGIN(compile)
NAMESPACE_BEGIN(runtime)
class scope;
NAMESPACE_END(runtime)

struct type : public compile::object
{
	type(int32 id = -1, int32 s = 0)
		: tid(id)
        , tsize(s)
        , defvalue(NULL)
		, content(NULL)
	{}

    /* overwrite */ virtual _Str to_string() const
    {
        return stringX::format("type<%d>", tid);
    }

	int32 tid; // type id
	byte* defvalue; // default value
	int32 tsize; // value's default size
	runtime::scope* content; // if type is a class/struct/enum/union..., 
			// content is the scope of the class/...
};

struct function_type : public type
{
	//struct param
	//{
	//	const type* ptype;
	//	const _Str pname;
	//};

    function_type(int32 id = -1)
        : type(id)
    {}

    kog::smart_vector<const type*> params_type;
    const type* return_type;
	void* more;
};

class typesystem : public kog::singleton<typesystem>
{
public:
    typesystem();
    virtual ~typesystem();
public:
    // check if exist same function type, if existed, return it, or create a new type and return it
    const function_type* new_func_type(int32 nparams, const type* params[], const type* return_type);
public:
    const type* type_type() const;
    const type* operator_type() const;
    const type* functiontype_type() const;
    const type* word_type() const;
    const type* int_type() const;
    const type* float_type() const;
public:
    const function_type* get_func_type(int32 nparams, const type* params[]) const;

	const type* gettype(int32 tid) const
	{
		return types_.at(tid);
	}
private:
	std::deque<type*> types_;
};

NAMESPACE_END(compile)

#endif
