#include "type.h"
#include <scerror.h>

using namespace compile;

//static type _g_int_type(0);
//static type _g_double_type(1);
//static type _g_operator_type(2);
//static type _g_function_type(3);

typesystem::typesystem()
{
    types_.push_back(new type(0)); // type_type
    types_.push_back(new type(1)); // operator_type
    types_.push_back(new type(2)); // function_type
    types_.push_back(new type(3)); // word_type
    types_.push_back(new type(4)); // int_type
    types_.push_back(new type(5)); // double_type
}

typesystem::~typesystem()
{
    for (std::deque<type*>::iterator iter = types_.begin(); iter != types_.end(); ++ iter)
    {
        delete *iter;
    }

    types_.clear();
}

const type* typesystem::type_type() const
{
    return types_[0];
}

const type* typesystem::int_type() const
{
    return types_[4];
}

const type* typesystem::double_type() const
{
    return types_[5];
}

const type* typesystem::operator_type() const
{
    return types_[1];
}

const type* typesystem::functiontype_type() const
{
    return types_[2];
}

const type* typesystem::word_type() const
{
    return types_[3];
}

const function_type* typesystem::get_func_type(int32 nparams, const type* params[]) const
{
    const function_type* pfound = NULL;
    for (std::deque<type*>::const_iterator iter = types_.begin(); iter != types_.end(); ++ iter)
    {
        if ((*iter)->get_type() != typeid(function_type)) continue;
        
        const function_type* ft = (*iter)->as<function_type>();
        if (ft->params_type.size() == nparams &&
                std::equal(ft->params_type.begin(), ft->params_type.end(), params))
        {
            pfound = ft;
            break;
        }
    }
    return pfound;
}

const function_type* typesystem::new_func_type(int32 nparams, const type* params[], const type* return_type)
{
    const function_type* pfound = get_func_type(nparams, params);
    if (pfound == NULL)
    {
        function_type* pft = new function_type((int32)types_.size());
        pft->return_type = return_type;
        pft->params_type.reset(nparams);
        std::copy(params, params + nparams, pft->params_type.begin());
        pfound = pft;
    }
    else if (pfound->return_type != return_type)
    {
        fire("found existing function type, but return_type is not equal");
    }
    return pfound;
}
