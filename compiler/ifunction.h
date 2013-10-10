// ifunction.h : production function
//

#ifndef _IFUNCTION_H_SC_
#define _IFUNCTION_H_SC_

#include <macros.h>
#include <basic_types.h>
#include <automachine.h>

class compiler;
NAMESPACE_BEGIN(compile)
//struct type;
struct object;
NAMESPACE_BEGIN(runtime)
struct tuple;
struct operation;
class variable;
class arraylist;
struct type;
//typedef compile::type type;
class scope;
class typesystem;
NAMESPACE_END(runtime)

class ifunction
{
public:
	//typedef runtime::buildinvalue<int> int_value;
    typedef automachine::machine_meta machine_meta;
    //typedef lalr1machine::lalr1meta compileitem;
public:
    virtual machine_meta* operator()(machine_meta*const* metas, int C, machine_meta* result) = 0;
public:
	virtual void entry();
	virtual void leave();
protected:
	// build-in functions
	runtime::variable* enter_variable(const tstring& name, const runtime::type* var_type, 
		runtime::variable* init = NULL, runtime::scope* varenv = NULL);
	//runtime::variable* enter_temporary(runtime::variable* var);
	tstring get_temporary_name();
	const runtime::type* create_type(const runtime::type* basetype, runtime::arraylist* ptarray, runtime::variable* init);
	const runtime::type* create_type(const tstring& name, size_t size);
	runtime::variable* find(const tstring& txt);
	runtime::variable* find(const tstring& txt, runtime::scope* find_scope);
	runtime::variable* temporary(const runtime::type* var_type);
	runtime::variable* temporary(const runtime::variable* var);
	runtime::variable* temporary(const tstring& txt, const runtime::type* var_type);
	runtime::variable* temporary(int32 x);
	runtime::arraylist* tolist(object* first, ...);
	runtime::arraylist* union_list(runtime::arraylist* v1, runtime::arraylist* v2);
	runtime::scope* get_curenv();
	//runtime::variable* str2var(const tstring& txt, const runtime::type* var_type, runtime::variable*& result);
	runtime::tuple* add_tuple(const tstring& oper, const object* src1 = NULL, const object* src2 = NULL, const object* dst = NULL);
	runtime::tuple* temporary_tuple(const tstring& oper, const object* src1 = NULL, const object* src2 = NULL, const object* dst = NULL);
	runtime::variable* ary_ref(runtime::variable* v, runtime::variable* idx, runtime::variable*& res);
	runtime::variable* ref(runtime::variable* v, int d, runtime::variable*& res);
	runtime::variable* mem_ref(runtime::variable* v, const tstring& mem, runtime::variable*& res);
	runtime::variable* ptr_ref(runtime::variable* v, const tstring& mem, runtime::variable*& res);
	const runtime::type* get_result_type(const tstring& oper, const runtime::type* arg1, const runtime::type* arg2);
	runtime::type* get_result_type(const tstring& oper, const runtime::type* arg1, const tstring& membername);
	bool can_convert(const runtime::type* from_type, const runtime::type* to_type);
protected:
	compiler* the_compiler;
	runtime::scope* cur_scope;
	runtime::typesystem* the_types;
private:
	const runtime::type* ptr_type(const runtime::type* basetype, object** v, size_t len);
	const runtime::type* arr_type(const runtime::type* basetype, object** v, size_t len, runtime::variable* init);
	size_t get_arr_len(const runtime::type* ptype, runtime::variable* init);
public:
	int32 prod_id; // production id
};

NAMESPACE_END(compile)
#endif
