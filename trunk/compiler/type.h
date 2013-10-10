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

struct type : public compile::object
{
	type(const tstring& type_name, int32 id = -1, int32 s = 0)
		: tid(id)
        , tsize(s)
        , defvalue(NULL)
		, content(NULL)
		, name(type_name)
		, const_type(NULL)
		, ptr_type(NULL)
		, base_type(NULL)
	{}

    /* overwrite */ virtual _Str to_string() const
    {
        //return stringX::format("type<%d>", tid);
		return name;
    }

	//const type* get_pointer_type() const;

	//const type* get_reference_type() const;
	

	const type* const_type;
	const type* ptr_type;
	const type* ref_type;
	const type* base_type;

	int32 tid; // type id
	byte* defvalue; // default value
	int32 tsize; // value's default size
	runtime::scope* content; // if type is a class/struct/enum/union..., 
			// content is the scope of the class/...
	tstring name; // name of the type
};

struct function_type : public type
{
    function_type(const tstring& name, int32 id = -1)
        : type(name, id)
    {}

	static tstring packname(int32 nparams, const type* params[], const type* return_type);

    kog::smart_vector<const type*> params_type;
    const type* return_type;
	void* more;
};

class array_type : public type
{
public:
	array_type(const type* base_type, size_t arraylen, int32 sid = -1);
public:
	MEMBER_VARIABLE_GET(size_t, arraylen, arrlen_);
public:
	static tstring get_array_type_name(const type& base_type, size_t arraylen);
public:
	// get count of all items
	size_t get_count() const;
protected:
	size_t arrlen_;
};

class typesystem : public kog::singleton<typesystem>
{
public:
    typesystem();
    virtual ~typesystem();
public:
	void init_buildin_types();
public:
    // check if exist same function type, if existed, return it, or create a new type and return it
    const function_type* new_func_type(int32 nparams, const type* params[], const type* return_type);
public:
	const type* get_const_type(const type* t);
	const type* get_array_type(const type* t, size_t array_size);
	const type* get_ptr_type(const type* t);
	const type* get_ref_type(const type* t);
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

// build-in types
extern const type* int_type;
extern const type* float_type;
extern const type* double_type;
extern const type* char_type;
extern const type* string_type;
extern const type* type_type;
extern const type* tuple_type;
extern const type* complex_variable_type;
extern const type* type_variable_type;
extern const type* pointer_variable_type;

NAMESPACE_END(runtime)
NAMESPACE_END(compile)

#endif
