// debug.h: assert
//

#ifndef _DEBUG_H_KOG_
#define _DEBUG_H_KOG_

#include <scerror.h>

NAMESPACE_BEGIN(kog)

struct assert
{
    template<typename _Tx>
    static void are_equal(const _Tx& expect, const _Tx& actual)
    {
#if (defined _DEBUG) || (defined DEBUG)
        if (!(expect == actual))
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