// funcparams.h: function params
//

#ifndef _FUNCTION_PARAMS_H_SC_
#define _FUNCTION_PARAMS_H_SC_

#include "object.h"
#include <cstdarg>

NAMESPACE_BEGIN(compile)
NAMESPACE_BEGIN(runtime)

struct funcparamlist : public variable 
{
    typedef kog::smart_vector<object*> vec_params;

    funcparamlist()
    {}

    funcparamlist(object* param)
        : params_(&param, 1)
    {}

    funcparamlist(object* param1, object* param2)
        : params_(2)
    {
        params_[0] = param1;
        params_[1] = param2;
    }

    funcparamlist(int n, ...)
        : params_(n)
    {
		va_list argptr;
		va_start(argptr, n);
        for (int i = 0; i < n; ++ i)
        {
            params_[i] = va_arg(argptr, object*);
        }
		va_end(argptr);
    }

    /* overwrite */ virtual _Str to_string() const
    {
        stringX::basic_stringbuffer<_Str::value_type> tmp;
        const _Str::value_type* sep = "[";
        for(vec_params::const_iterator iter = params_.begin(); iter != params_.end(); ++ iter)
        {
            tmp<<sep<<((*iter) != NULL ? (*iter)->to_string() : "<null>");
            sep = ", ";
        }
        tmp<<"]";
        return tmp.str();
    }

    vec_params params_;
};

NAMESPACE_END(runtime)
NAMESPACE_END(compile)
#endif
