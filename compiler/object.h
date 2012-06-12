// object.h: basic type
//

#ifndef _OBJECT_H_SC_
#define _OBJECT_H_SC_

#include <macros.h>
#include <typeinfo>

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
};

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

NAMESPACE_END(compile)

#endif
