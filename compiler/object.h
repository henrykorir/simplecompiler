// object.h: basic type
//

#ifndef _OBJECT_H_SC_
#define _OBJECT_H_SC_

#include <macros.h>
#include <basic_types.h>
#include <typeinfo>
#include <vector>

NAMESPACE_BEGIN(compile)

struct object
{
    virtual ~object()
    {
    }

    const std::type_info& get_type() const
    {
        return typeid(*this);
    }

    template<typename _Ty> _Ty* as()
    {
        return dynamic_cast<_Ty*>(this);
    }

    template<typename _Ty> const _Ty* as() const
    {
        return dynamic_cast<const _Ty*>(this);
    }

    virtual _Str to_string() const
    {
        return get_type().name();
    }
};

//class objectptrlist : public std::vector<object*>
//{
//};
typedef std::vector<object*> objectptrlist;

template<typename _Tx, typename _Ty> const _Tx* as(const _Ty* _v)
{
    return dynamic_cast<const _Tx*>(_v);
}

template<typename _Tx, typename _Ty> _Tx* as(_Ty* _v)
{
    return dynamic_cast<_Tx*>(_v);
}

template<typename _Tx, typename _Ty> const _Tx& as(const _Ty& _v)
{
    return dynamic_cast<const _Tx&>(_v);
}

template<typename _Tx, typename _Ty> _Tx& as(_Ty& _v)
{
    return dynamic_cast<_Tx&>(_v);
}

template<typename _Tx, typename _Ty> bool is(const _Ty* _v)
{
    return as<_Tx, _Ty>(_v) != NULL;
}

template<typename _Tx, typename _Ty> bool is(_Ty* _v)
{
    return as<_Tx, _Ty>(_v) != NULL;
}

template<typename _Tx, typename _Ty> bool is(const _Ty& _v)
{
	bool isOk = false;
    try{
        as<_Tx, _Ty>(_v);
        isOk = true;
    }catch(...){
    }
	return isOk;
}

// with a flag
struct flagobject : public object
{
    flagobject(int i = 0)
        : flag(i)
    {}

    int flag;
};

NAMESPACE_END(compile)

#endif
