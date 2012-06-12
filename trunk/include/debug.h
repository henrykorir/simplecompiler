// debug.h: assert
//

#ifndef _DEBUG_H_KOG_
#define _DEBUG_H_KOG_

#include <scerror.h>

NAMESPACE_BEGIN(kog)

struct assert
{
    template<typename _Tx>
    static void are_equal(const _Tx& v1, const _Tx& v2)
    {
#if (defined _DEBUG) || (defined DEBUG)
        if (v1 != v2)
        {
            fire("not equal!");
        }
#endif
    }

    static void is_true(bool b)
    {
#if (defined _DEBUG) || (defined DEBUG)
        if (!b)
        {
            fire("not true");
        }
#endif
    }
};

NAMESPACE_END(kog)

#endif
