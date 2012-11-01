#include "type.h"
#include <scerror.h>
#include <stringX.h>

using namespace compile;

tstring function_type::packname(int32 nparams, const type* params[], const type* return_type)
{
	stringX::string_builder strb;
	strb<<"_R"<<return_type->name<<"_Z";
	foreach (const type* p, params, params + nparams)
	{
		strb<<p->name;
	}
	return strb.str();
}

typesystem::typesystem()
{
    types_.push_back(new type("$type_type$", 0, 4)); // type_type
    types_.push_back(new type("$operator_type$", 1, 4)); // operator_type
    types_.push_back(new type("$function_type$", 2, 4)); // function_type
    types_.push_back(new type("$word_type$", 3, 4)); // word_type
    //types_.push_back(new type("int", 4, sizeof(int32))); // int_type
    //types_.push_back(new type("float", 5, sizeof(float))); // float_type
    //types_.push_back(new type("double", 6, sizeof(double))); // float_type
    //types_.push_back(new type("short", 7, sizeof(short))); // float_type
    //types_.push_back(new type("char", 8, sizeof(char))); // float_type
    //types_.push_back(new type("long", 9, sizeof(long))); // float_type
    //types_.push_back(new type("long long", 10, sizeof(long long))); // float_type
    //types_.push_back(new type("unsigned char", 11, sizeof(unsigned char))); // float_type
    //types_.push_back(new type("unsigned short", 12, sizeof(unsigned short))); // float_type
    //types_.push_back(new type("unsigned int", 13, sizeof(unsigned int))); // float_type
    //types_.push_back(new type("unsigned long", 14, sizeof(unsigned long))); // float_type
    //types_.push_back(new type("unsigned long long", 15, sizeof(unsigned long long))); // float_type
}

typesystem::~typesystem()
{
	foreach (type* t, types_.begin(), types_.end())
    {
		delete t;
    }

    types_.clear();
	name2type_.clear();
}

const type* typesystem::type_type() const
{
    return types_[0];
}

//const type* typesystem::int_type() const
//{
//    return types_[4];
//}
//
//const type* typesystem::float_type() const
//{
//    return types_[5];
//}

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

const function_type* typesystem::new_func_type(int32 nparams, const type* params[], const type* return_type)
{
	const tstring& ftname = function_type::packname(nparams, params, return_type);
	const type* pfound = get_type(ftname);
    if (pfound == NULL)
    {
        function_type* pft = new_type<function_type>(ftname);
        pft->return_type = return_type;
        pft->params_type.reset(nparams);
		pft->tsize = 4;
        std::copy(params, params + nparams, pft->params_type.begin());
        pfound = pft;
    }

    return as<function_type>(pfound);
}

const type* typesystem::get_type(int32 tid) const
{
	return types_.at(tid);
}

const type* typesystem::get_type(const tstring& name) const
{
	logstring("find type %s", name.c_str());
	std::map<tstring, type*>::const_iterator iterfind = name2type_.find(name);
	if (iterfind != name2type_.end()) return iterfind->second;
	return NULL;
}


