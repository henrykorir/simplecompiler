// type.h: types
//

#ifndef _TYPES_H_SC_
#define _TYPES_H_SC_

#include "object.h"
#include <arrayX.h>
#include <stringXF.h>
#include <singleton.h>
#include <deque>
#include <map>
#include <basic_types.h>

NAMESPACE_BEGIN(compile)
NAMESPACE_BEGIN(runtime)
class scope;
NAMESPACE_END(runtime)

struct type : public compile::object
{
	type(const tstring& type_name, int32 id = -1, int32 s = 0)
		: tid(id)
        , tsize(s)
        , defvalue(NULL)
		, content(NULL)
		, name(type_name)
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
	tstring name; // name of the type
};

struct function_type : public type
{
	//struct param
	//{
	//	const type* ptype;
	//	const _Str pname;
	//};

    function_type(const tstring& name, int32 id = -1)
        : type(name, id)
    {}

	static tstring packname(int32 nparams, const type* params[], const type* return_type);

    kog::smart_vector<const type*> params_type;
    const type* return_type;
	void* more;
};

class pointer_type : public type
{
	pointer_type(const tstring& name, const type* basictype = NULL, int32 id = -1)
		: basic_type(basictype)
		, type(name, id, 4)
	{}

	const type* basic_type;
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
    const type* word_type() const;
    const type* functiontype_type() const;
public:
	// get type from name
	const type* get_type(const tstring& name) const;
	// get type from tid
	const type* get_type(int32 tid) const;
public:
	template<typename _Type> _Type* new_type(const tstring& name)
	{
		const type* pt = get_type(name);
		if (pt != NULL) return (_Type*)as<_Type>(pt);
		_Type* pn = new _Type(name);
		pn->tid = (int32)types_.size();
		types_.push_back(pn);
		name2type_[name] = pn;
		return pn;
	}
private:
	std::deque<type*> types_;
	std::map<tstring, type*> name2type_;
};

NAMESPACE_END(compile)

#endif
