#include "type.h"
#include <scerror.h>
#include <stringX.h>
#include <functionalX.h>
#include "object.h"

#include "type_helper.h"

using namespace compile;
using namespace compile::runtime;

const type* compile::runtime::int_type = NULL;
const type* compile::runtime::float_type = NULL;
const type* compile::runtime::double_type = NULL;
const type* compile::runtime::char_type = NULL;
const type* compile::runtime::string_type = NULL;
const type* compile::runtime::type_type = NULL;
const type* compile::runtime::tuple_type = NULL;
const type* compile::runtime::complex_variable_type = NULL;
const type* compile::runtime::type_variable_type = NULL;
const type* compile::runtime::pointer_variable_type = NULL;

array_type::array_type(const type* btype, size_t arrlen, int32 sid)
: type("", sid)
, arrlen_(arrlen)
{
	if (btype == NULL) fire("invalidate base_type");
	base_type = btype;
	tsize = arrlen * btype->tsize;
	name = get_array_type_name(*btype, arrlen);
}

tstring array_type::get_array_type_name(const type& btype, size_t arrlen)
{
	std::string name;
	if (!is<array_type>(&btype))
	{
		name = btype.name + stringX::format(" (*)[%d]", (int)arrlen);
	}
	else
	{
		size_t pos = btype.name.rfind(" (*)");
		if (pos != tstring::npos)
		{
			name = btype.name;
			name.replace(pos, 4, stringX::format(" (*)[%d]", (int)arrlen));
		}
		else fire("must be some error! will not occure");
	}

	return name;	
}

size_t array_type::get_count() const
{
	size_t n = 1;
	const type* p = this;
	while (_is_array_type(p))
	{
		n *= as<array_type>()->arraylen();
		p = p->base_type;
	}
	return n;
}

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
}

void typesystem::init_buildin_types()
{
	types_.clear();
	name2type_.clear();

	int32 type_index = 0;
	types_.push_back(new type("$type_type$", type_index++, 4)); // type_type
    types_.push_back(new type("$operator_type$", type_index++, 4)); // operator_type
    types_.push_back(new type("$function_type$", type_index++, 4)); // function_type
    types_.push_back(new type("$word_type$", type_index++, 4)); // word_type
    types_.push_back(new type("int", type_index++, sizeof(int32))); // int_type
    types_.push_back(new type("float", type_index++, sizeof(float))); // float_type
    types_.push_back(new type("double", type_index++, sizeof(double))); // double_type
    types_.push_back(new type("short", type_index++, sizeof(short))); // short_type
    types_.push_back(new type("char", type_index++, sizeof(char))); // char_type
    types_.push_back(new type("long", type_index++, sizeof(long))); // long_type
    types_.push_back(new type("long int", type_index++, sizeof(long int))); // long_long_type
    types_.push_back(new type("long long", type_index++, sizeof(long long))); // long_long_type
    types_.push_back(new type("long double", type_index++, sizeof(long double))); // long_double_type
    types_.push_back(new type("unsigned char", type_index++, sizeof(unsigned char))); // uchar_type
    types_.push_back(new type("unsigned short", type_index++, sizeof(unsigned short))); // ushort_type
    types_.push_back(new type("unsigned int", type_index++, sizeof(unsigned int))); // uint_type
    types_.push_back(new type("unsigned long", type_index++, sizeof(unsigned long))); // ulong_type
    types_.push_back(new type("unsigned long int", type_index++, sizeof(unsigned long int))); // ulong_int_type
    types_.push_back(new type("unsigned long long", type_index++, sizeof(unsigned long long))); // ulong_long_type
	types_.push_back(new type("$string_type$", type_index++, sizeof(char*))); // string_type
	types_.push_back(new type("$tuple_type$", type_index++, sizeof(void*))); // tuple_type
	types_.push_back(new type("$complex_variable$", type_index++, sizeof(void*))); // complex_type
	types_.push_back(new type("$pointer_variable$", type_index++, sizeof(void*))); // pointer_variable_type

	for (std::deque<type*>::iterator iter = types_.begin(); iter != types_.end(); ++ iter)
	{
		name2type_[(*iter)->name] = *iter;
	}

	compile::runtime::type_type = get_type("$type_type$");
	compile::runtime::int_type = get_type("int");
	compile::runtime::float_type = get_type("float");
	compile::runtime::double_type = get_type("double");
	compile::runtime::char_type = get_type("char");
	compile::runtime::string_type = get_type("$string_type$");
	compile::runtime::tuple_type = get_type("$tuple_type$");
	compile::runtime::complex_variable_type = get_type("$complex_variable$");
	compile::runtime::type_variable_type = type_type;
	compile::runtime::pointer_variable_type = get_type("$pointer_variable$");
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


const type* typesystem::get_const_type(const type* t)
{
	if (t == NULL) fire("null type");
	if (t->const_type == NULL)
	{ // 
		type* p = types_.at(t->tid);
		if (p != t) fire("not this typesystem's type");
		p->const_type = new type(p->name + " const", types_.size(), p->tsize);
		((type*)(p->const_type))->base_type = p;
		types_.push_back((type*)p->const_type);
		name2type_[p->const_type->name] = (type*)p->const_type;
		logstring("create const type(%s) of (%s)", p->const_type->name.c_str(), p->name.c_str());
	}

	return t->const_type;
}

const type* typesystem::get_array_type(const type* t, size_t array_size)
{
	if (t == NULL) fire("null type");
	type* p = types_.at(t->tid);
	if (p != t) fire("not this typesystem's type");
	const tstring arr_typename = array_type::get_array_type_name(*t, array_size);
	std::map<tstring, type*>::iterator iter = name2type_.find(arr_typename);
	type* ap = NULL;
	if (iter == name2type_.end())
	{
		ap = new array_type(t, array_size, (int)types_.size());
		types_.push_back(ap);
		name2type_[ap->name] = ap;
		logstring("create array type(%s) of (%s)", ap->name.c_str(), p->name.c_str());
	}
	else ap = iter->second;

	return ap;
}

const type* typesystem::get_ptr_type(const type* t)
{
	if (t == NULL) fire("null type");
	if (t->ptr_type == NULL)
	{
		type* p = types_.at(t->tid);
		if (p != t) fire("not this typesystem's type");
		p->ptr_type = new type(p->name + " *", types_.size(), 4);
		((type*)(p->ptr_type))->base_type = p;
		types_.push_back((type*)p->ptr_type);
		name2type_[p->ptr_type->name] = (type*)p->ptr_type;
		logstring("create ptr type(%s) of (%s)", p->ptr_type->name.c_str(), p->name.c_str());
	}

	return t->ptr_type;
}

const type* typesystem::get_ref_type(const type* t)
{
	if (t == NULL) fire("null type");
	if (t->ref_type == NULL)
	{
		type* p = types_.at(t->tid);
		if (p != t) fire("not this typesystem's type");
		p->ref_type = new type(p->name + " &", types_.size(), 4);
		((type*)(p->ref_type))->base_type = p;
		types_.push_back((type*)p->ref_type);
		name2type_[p->ref_type->name] = (type*)p->ref_type;
		logstring("create ref type(%s) of (%s)", p->ref_type->name.c_str(), p->name.c_str());
	}

	return t->ref_type;
}
