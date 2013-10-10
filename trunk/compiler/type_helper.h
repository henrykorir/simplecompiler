// type_helper.h: type helper function
//

#ifndef _TYPE_HELPER_H_SC_
#define _TYPE_HELPER_H_SC_

#include "type.h"

NAMESPACE_BEGIN(compile)
NAMESPACE_BEGIN(runtime)

inline const tchar* get_typeinfo(const type* t)
{
	return t == NULL ? "(null)" : t->name.c_str();
}

inline const type* _max_type(const type* arg1, const type* arg2)
{
#define _IS_MAXTYPE(type_name) \
	{ \
		const type* atype = typesystem::instance().get_type(type_name); \
		if (atype == NULL) fire("not found type(%s)", type_name); \
		if (arg1 == atype || arg2 == atype) return atype; \
	}

	_IS_MAXTYPE("long double");
	_IS_MAXTYPE("double");
	_IS_MAXTYPE("float");
	_IS_MAXTYPE("unsigned long long");
	_IS_MAXTYPE("long long");
	_IS_MAXTYPE("unsigned long int");
	_IS_MAXTYPE("long int");
	_IS_MAXTYPE("unsigned long");
	_IS_MAXTYPE("long");
	_IS_MAXTYPE("unsigned int");
	_IS_MAXTYPE("int");
	_IS_MAXTYPE("unsigned short");
	_IS_MAXTYPE("short");
	_IS_MAXTYPE("unsigned char");
	_IS_MAXTYPE("char");

	return NULL;
}

inline bool _is_unsigned_type_pair(const type* arg1, const type* arg2)
{
#define _IS_UNSIGNED_PAIR(type_name) \
	{ \
		const type* atype = typesystem::instance().get_type(type_name); \
		const type* atype_unsigned = typesystem::instance().get_type(std::string("unsigned ") + type_name); \
		if (atype == NULL || atype_unsigned == NULL) fire("not found type(%s)", type_name); \
		if ((arg1 == atype && arg2 == atype_unsigned) || (arg1 == atype_unsigned && arg2 == atype)) return true; \
	}
	_IS_UNSIGNED_PAIR("long long");
	_IS_UNSIGNED_PAIR("long int");
	_IS_UNSIGNED_PAIR("long");
	_IS_UNSIGNED_PAIR("int");
	_IS_UNSIGNED_PAIR("short");
	_IS_UNSIGNED_PAIR("char");

	return false;
}

inline bool _is_int_type(const type* input_type)
{
#define _IS_INTTYPE(type_name) \
	{ \
		const type* atype = typesystem::instance().get_type(type_name); \
		if (atype == NULL) fire("not found type(%s)", type_name); \
		if (input_type == atype) return true; \
	}
	_IS_INTTYPE("unsigned long long");
	_IS_INTTYPE("long long");
	_IS_INTTYPE("unsigned long int");
	_IS_INTTYPE("long int");
	_IS_INTTYPE("unsigned long");
	_IS_INTTYPE("long");
	_IS_INTTYPE("unsigned int");
	_IS_INTTYPE("int");
	_IS_INTTYPE("unsigned short");
	_IS_INTTYPE("short");
	_IS_INTTYPE("unsigned char");
	_IS_INTTYPE("char");
	
	return false;
}

inline bool _is_const_type(const type* atype)
{
	return atype != NULL && atype->base_type != NULL &&
		atype == atype->base_type->const_type;
}

inline bool _is_ptr_type(const type* atype)
{
	return atype != NULL && atype->base_type != NULL &&
		atype == atype->base_type->ptr_type;
}

inline bool _is_ref_type(const type* atype)
{
	return atype != NULL && atype->base_type != NULL &&
		atype == atype->base_type->ref_type;
}

inline bool _is_array_type(const type* atype)
{
	return atype != NULL && atype->base_type != NULL &&
		is<array_type>(atype);
}

NAMESPACE_END(runtime)
NAMESPACE_END(compile)
#endif
