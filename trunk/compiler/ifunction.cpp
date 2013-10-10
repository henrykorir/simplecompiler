#include "ifunction.h"
#include "variable.h"
#include "type.h"
#include "scope.h"
#include "tuple.h"
#include "arraylist.h"
#include "compiler.h"
#include "type_helper.h"

#include <typeinfoname.h>

using namespace compile;
using namespace compile::runtime;

void ifunction::entry()
{
	logstring("entry production %s", kog::typeinfo_name(typeid(*this).name()).c_str());
	the_compiler = &compiler::instance();
	the_types = &typesystem::instance();
	cur_scope = the_compiler->get_iml()->current_scope();
}

void ifunction::leave()
{
	the_compiler = NULL;
	the_types = NULL;
	cur_scope = NULL;
	logstring("leave production %s", kog::typeinfo_name(typeid(*this).name()).c_str());
}

const runtime::type* ifunction::create_type(const runtime::type* basetype, arraylist* ptarray, variable* init)
{
	if (basetype == NULL) fire("empty base type");
	// find ptr and array
	size_t sep = 0;
	for (; sep < ptarray->size(); ++ sep)
	{
		if (!is<string_value>((*ptarray)[sep]))
		{
			fire("invalidate info");
		}
		if ((*ptarray)[sep]->to_string().empty()) break;
	}

	if (sep == ptarray->size()) fire("invalidate type, not found symbol position");

	const runtime::type* ptype = basetype;
	if (ptarray->size() > 0) ptype = ptr_type(ptype, &((*ptarray)[0]), sep);
	if (ptype == NULL) fire("get ptr type failed!");
	if (sep + 1 < ptarray->size()) ptype = arr_type(ptype, &((*ptarray)[sep+1]), ptarray->size() - sep - 1, init);
	if (ptype == NULL) fire("get arr type failed!");
	
	logstring("get type result(%s)", ptype->name.c_str());
	return ptype;
}

const runtime::type* ifunction::ptr_type(const runtime::type* ptype, object** v, size_t len)
{
	bool pre_is_const = false;
	for (size_t i = 0; i < len; ++ i)
	{
		tstring tmp = v[i]->to_string();
		if (tmp == "const" && pre_is_const) fire("tow const"); 
		pre_is_const = (tmp == "const");
	}
	for (size_t i = 0; i < len; ++ i)
	{
		tstring tmp = v[i]->to_string();
		if (tmp == "const") ptype = the_types->get_const_type(ptype);
		else if (tmp == "*") ptype = the_types->get_ptr_type(ptype);
		else fire("invalidate string(%s)", tmp.c_str());
	}

	return ptype;
}

size_t get_matrix_size(const arraylist* al)
{
	std::stack<const arraylist*> alstack;
	alstack.push(al);
	size_t n = 0;
	while (!alstack.empty())
	{
		const arraylist* p = alstack.top();
		alstack.pop();
		
		if (p == NULL) continue;
		foreach (const object* x, ((arraylist*)p)->begin(), ((arraylist*)p)->end())
		{
			if (is<arraylist>(x)) alstack.push(as<arraylist>(x));
			else ++ n;
		}
	}
	return n;
}

size_t ifunction::get_arr_len(const runtime::type* ptype, runtime::variable* init)
{
	arraylist& al = *(as<arraylist>(init));
	if (al.size() == 0) return 0;

	size_t n = is<array_type>(ptype) ? as<array_type>(ptype)->get_count() : 1; // count of subarray's elements
	if (n == 0)
	{
		logwarning("array size is 0");
		return 0;
	}

	bool has_sublist = is<arraylist>(al);
	bool all_has_sublist = has_sublist;
	for (size_t i = 1; i < al.size(); ++ i)
	{
		all_has_sublist &= !is<arraylist>(al[i]);
		has_sublist |= is<arraylist>(al[i]);
	}
		
	size_t arr_len = 0;
	if (has_sublist & all_has_sublist)
	{ // sublist's count is array length
		arr_len = al.size();
	}
	else if (!has_sublist)
	{ // don't has sublist
		arr_len = (al.size() + n - 1) / n;
	}
	else
	{ // has sublist and 
		arr_len = 0;
		size_t alone_count = 0;
		bool is_not_good = false;
		for (size_t i = 0; i < al.size(); ++ i)
		{
			if (!is<arraylist>(al[i])) ++ alone_count;
			else if (is_not_good) 
			{
				alone_count += get_matrix_size(as<arraylist>(al[i]));
			}
			else
			{
				if (alone_count % n == 0) arr_len += (alone_count / n) + 1;
				else
				{ // mark all items as alone
					logwarning("excess elements in scalar initializer");
					is_not_good = true;
					alone_count += get_matrix_size(as<arraylist>(al[i]));
				}
			}
		}
		arr_len += (alone_count + n - 1) / n;
	}

	return arr_len;
}

const runtime::type* ifunction::arr_type(const runtime::type* ptype, object** v, size_t len, runtime::variable* init)
{
	// get array type
	for (size_t i = 0; i < len; ++ i)
	{ // make sure int_value
		if (!is<variable>(v[i]))
		{
			fire("invalidate array info(%s)", v[i] == NULL ? "(null)" : v[i]->to_string().c_str());
		}
		const variable* p = as<variable>(v[i]);
		if (p->initVar() != NULL && p->vtype() == int_type)
		{
			int arrlen = *(int*)(p->initVar()->initv);
			if (arrlen == -1)
			{
				if (i != len - 1) fire("can't get array size");
				else if (init == NULL) fire("init list is empty, can't get array size");
				else if (!is<arraylist>(init)) fire("init list is not a list, (this error will not occure)");
				// get array length from init list
				arrlen = (int)get_arr_len(ptype, init);
				ptype = the_types->get_array_type(ptype, arrlen);
				logstring("result type %s", ptype == NULL ? "(null)" : ptype->name.c_str());
			}
			else
			{
				ptype = the_types->get_array_type(ptype, arrlen);
				logstring("result type %s", ptype == NULL ? "(null)" : ptype->name.c_str());
			}
		}
		else
		{
			fire("not implement");
		}
	}

	return ptype;
}

variable* ifunction::enter_variable(const tstring& name, const runtime::type* var_type, runtime::variable* init, scope* varenv)
{
	if (var_type == NULL) fire("null type");
	if (name.empty()) fire("name is empty!");

	if (varenv == NULL) varenv = cur_scope;
	variable* var = varenv->entry_variable(name, var_type, 0);

	if (var == NULL)
	{
		fire("enter variable(%s) in scope(%s) failed", name.c_str(), cur_scope->name().c_str());
	}

	if (init != NULL)
	{
		add_tuple("assign", init, NULL, var);
	}
	return var;
}

tstring ifunction::get_temporary_name()
{
	return cur_scope->get_temp_name();
}

//runtime::variable* enter_temporary(runtime::variable* var)
//{
//	if (var == NULL) fire("null variable");
//
//	variable* x = cur_scope->entry_temporary(var);
//	if (x == NULL)
//	{
//		fire("enter temporary(0x%p:%s) in scope(%s) failed", var, obj2str(var), cur_scope->name().c_str());
//	}
//
//	return x;
//}

runtime::variable* ifunction::temporary(const tstring &txt, const runtime::type *var_type)
{
	variable* var = get_curenv()->entry_value(txt, var_type);
	logstring("add temporary value(%s), type is(%s)", txt.c_str(), var_type->name.c_str());
	return var;
}

runtime::variable* ifunction::temporary(int32 x)
{
	value* v = new value;
	v->size = (uint32)int_type->tsize;
	v->initv = (byte*)new int(x);
	variable* var = new variable(stringX::format("%d", x), int_type, get_curenv(), v);
	logstring("add temporary value(%d), type is(%s)", x, int_type->name.c_str());
	return var;
}

runtime::variable* ifunction::find(const tstring& txt)
{
	return find(txt, global_scope);
}

runtime::variable* ifunction::find(const tstring& txt, runtime::scope* find_scope)
{
	if (find_scope == NULL)
	{
		fire("input scope is null!");
	}

	variable* v = find_scope->find(txt);
	if (v == NULL)
	{
		fire("undefined symbol %s", txt.c_str());
	}
	
	return v;
}

runtime::variable* ifunction::temporary(const runtime::type* var_type)
{
	variable* var = new variable("#tmp#", var_type, cur_scope);
	return var;
}

runtime::variable* ifunction::temporary(const runtime::variable* var)
{
	variable* tmp = new variable(*var);
	return tmp;
}

runtime::scope* ifunction::get_curenv()
{
	return cur_scope;
}

runtime::tuple* ifunction::add_tuple(const tstring& oper, const object* src1, const object* src2, const object* dst)
{
	//runtime::tuple* atuple = temporary_tuple(oper, src1, src2, dst);
	runtime::operation op(oper);
	return cur_scope->entry_tuple(&op, src1, src2, dst);
}

runtime::tuple* ifunction::temporary_tuple(const tstring& oper, const object* src1, const object* src2, const object* dst)
{
	runtime::four_tuple* atuple = new runtime::four_tuple;
	atuple->dst = (variable*)dst;
	atuple->oper = operation(oper).oper;
	atuple->src1 = (variable*)src1;
	atuple->src2 = (variable*)src2;
	return atuple;
}

runtime::arraylist* ifunction::tolist(object* p, ...)
{
	runtime::arraylist* tmplist = new runtime::arraylist;
	va_list argptr;
	va_start(argptr, p);
	
	while (p != NULL)
	{
		//logstring("add to list 0x%p", p);
		(*tmplist) += p;
		p = va_arg(argptr, object*);
	}
	
	va_end(argptr);

	// print union list
	//std::string result;
	//for (arraylist::iterator iter = tmplist->begin(); iter != tmplist->end(); ++ iter)
	//{
	//	result += (iter == tmplist->begin() ? "" : ", ") + (*iter)->to_string();
	//}
	//logstring("result [%s]", result.c_str());

	return tmplist;
}

runtime::arraylist* ifunction::union_list(runtime::arraylist* v1, runtime::arraylist* v2)
{
	arraylist* tmp = new arraylist(*v1);
	(*tmp) += *v2;

	return tmp;
}

runtime::variable* ifunction::ary_ref(runtime::variable* v, runtime::variable* idx, runtime::variable*& res)
{
	return NULL;
}

runtime::variable* ifunction::mem_ref(runtime::variable* v, const tstring& mem, runtime::variable*& res)
{
	return NULL;
}

runtime::variable* ifunction::ref(runtime::variable* v, int d, runtime::variable*& res)
{
	return NULL;
}

runtime::variable* ifunction::ptr_ref(runtime::variable* v, const tstring& mem, runtime::variable*& res)
{
	return NULL;
}

const runtime::type* ifunction::get_result_type(const tstring& oper, const runtime::type* arg1, const runtime::type* arg2)
{
	if (_is_const_type(arg1)) arg1 = arg1->base_type;
	if (_is_const_type(arg2)) arg2 = arg2->base_type;
	bool is_ptr1 = _is_ptr_type(arg1);
	bool is_ptr2 = _is_ptr_type(arg2);
	bool is_ary1 = _is_array_type(arg1);
	bool is_ary2 = _is_array_type(arg2);
	if (oper == "plus" || oper == "minus" || oper == "multi" || oper == "divid")
	{

		if (arg1 == NULL || arg2 == NULL)
		{
			fire("get result type of %s(%s, %s) failed!", oper.c_str(), get_typeinfo(arg1), get_typeinfo(arg2));
		}
		
		if ((is_ptr1 || is_ary1) && (is_ptr2 || is_ary2))
		{
			fire("get result type of %s(%s, %s) failed!", oper.c_str(), get_typeinfo(arg1), get_typeinfo(arg2));
		}

		if (is_ptr1 && _is_int_type(arg2)) return arg1;
		if (is_ptr2 && _is_int_type(arg1)) return arg2;

		if (is_ary1 && _is_int_type(arg2)) return arg1;
		if (is_ary2 && _is_int_type(arg1)) return arg2;

		if (is_ptr1 || is_ptr2)
		{
			fire("get result type of %s(%s, %s) failed!", oper.c_str(), get_typeinfo(arg1), get_typeinfo(arg2));
		}

		if (arg1 == arg2) return arg1;

		if (_is_unsigned_type_pair(arg1, arg2))
		{
			return arg1;
		}
		const runtime::type* max_type =  _max_type(arg1, arg2);
		if (max_type == NULL)
		{
			fire("not support type(%s) and type(%s)", get_typeinfo(arg1), get_typeinfo(arg2));
		}
		return max_type;
	}
	else if (oper == "lt" || oper == "gt" || oper == "le" || oper == "ge" || oper == "ne" || oper == "eq")
	{
		if (arg1 == NULL || arg2 == NULL)
		{
			fire("get result type of %s(%s, %s) failed!", oper.c_str(), get_typeinfo(arg1), get_typeinfo(arg2));
		}

		if (is_ptr1 && is_ptr2)
		{
			return int_type;
		}

		if (is_ptr1 || is_ptr2)
		{
			fire("get result type of %s(%s, %s) failed!", oper.c_str(), get_typeinfo(arg1), get_typeinfo(arg2));
		}

		if (_is_unsigned_type_pair(arg1, arg2))
		{
			return int_type;
		}

		if (_max_type(arg1, arg2) == NULL)
		{
			fire("not support type(%s) and type(%s)", get_typeinfo(arg1), get_typeinfo(arg2));
		}
		return int_type;
	}
	else
	{
		fire("get result type of %s(%s, %s) failed!", oper.c_str(), get_typeinfo(arg1), get_typeinfo(arg2));
	}
	return NULL;
}

bool ifunction::can_convert(const runtime::type* from_type, const runtime::type* to_type)
{
	if (from_type == NULL || to_type == NULL)
	{
		fire("check convert type(%s) to type(%s) failed!", get_typeinfo(from_type), get_typeinfo(to_type));
	}

	if (from_type == to_type) return true;
	if (_is_unsigned_type_pair(from_type, to_type))
	{
		logwarning("convert from %s to %s", get_typeinfo(from_type), get_typeinfo(to_type));
		return true;
	}
	
	const runtime::type* max_type =  _max_type(from_type, to_type);
	if (max_type != NULL)
	{
		if (from_type == max_type)
		{
			logwarning("convert from %s to %s", get_typeinfo(from_type), get_typeinfo(to_type));
		}
		return  true;
	}

	return false;
}

runtime::type* ifunction::get_result_type(const tstring& oper, const runtime::type* arg1, const tstring& membername)
{
	return NULL;
}
