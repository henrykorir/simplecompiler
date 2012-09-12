#include <ifunction.h>
#include <interlanguage.h>
#include <compiler.h>
#include <funcparams.h>

using namespace compile;
using namespace compile::doc;
using namespace compile::runtime;

typedef lalr1machine::lalr1meta lalr1meta;

variable* check_isSymbol(scope* cs, object* v)
{
	if (v == NULL || !is<word>(v)) 
		return NULL;
	word* w = as<word>(v);
	variable* var = cs->find(w->txt);
	return var;
}

// A -> a
struct prod_func_default : public ifunction
{
    /*overwrite*/ virtual automachine::machine_meta* operator()(automachine::machine_meta*const* metas, int C, automachine::machine_meta* result)
    {
        const lalr1meta* m0 = (const lalr1meta*)(metas[0]);
        lalr1meta* ot = (lalr1meta*)(result);
        ot->content = m0->content;
        ot->ctype = m0->ctype;
        return result;
    }
};

// ValueDeclear -> Valuetype symbol
struct prod_func4 : public ifunction
{
    /*overwrite*/ virtual automachine::machine_meta* operator()(automachine::machine_meta*const* metas, int C, automachine::machine_meta* result)
    {
        interlanguage& iml = compiler::getiml();
        scope* s = iml.current_scope();
        const lalr1meta* m0 = (const lalr1meta*)(metas[0]);
        const lalr1meta* m1 = (const lalr1meta*)(metas[1]);
        lalr1meta* ot = (lalr1meta*)(result);
        const type* vt = as<type>(m0->content);
		variable* var = s->entry_variable(as<word>(m1->content)->txt, vt, 0); // new variable
		// set address of current value
		var->more() = (void*)"4(%ebp)";

        ot->content = var;
        ot->ctype = vt;
        return result;
    }
};

// Assignment -> symbol = AValue Op AValue
struct prod_func12 : public ifunction
{
    /*overwrite*/ virtual automachine::machine_meta* operator()(automachine::machine_meta*const* metas, int C, automachine::machine_meta* result)
    {
        interlanguage& iml = compiler::getiml();
        scope* s = iml.current_scope();
        const lalr1meta* m0 = (const lalr1meta*)(metas[0]);
        const lalr1meta* m2 = (const lalr1meta*)(metas[2]);
        const lalr1meta* m3 = (const lalr1meta*)(metas[3]);
        const lalr1meta* m4 = (const lalr1meta*)(metas[4]);
        lalr1meta* ot = (lalr1meta*)(result);

        variable* v = s->find(as<word>(m0->content)->txt);
		if (NULL == v) fire("undefined symbol " + as<word>(m0->content)->txt);

        s->entry_tuple(as<operation>(m3->content), m2->content, m4->content, v); // op, src1, src2, dst
        ot->content = v;
        ot->ctype = v->vtype();
        return result;
    }
};

// AValue -> symbol
struct prod_func13 : public ifunction
{
    /*overwrite*/ virtual automachine::machine_meta* operator()(automachine::machine_meta*const* metas, int C, automachine::machine_meta* result)
    {
        interlanguage& iml = compiler::getiml();
        scope* s = iml.current_scope();
        const lalr1meta* m0 = (const lalr1meta*)(metas[0]);
        lalr1meta* ot = (lalr1meta*)(result);

        variable* v = s->find(as<word>(m0->content)->txt);
		if (NULL == v) fire("undefined symbol " + as<word>(m0->content)->txt);
        ot->content = v;
        ot->ctype = v->vtype();

        return result;
    }
};

// AValue -> number
struct prod_func14 : public ifunction
{
    /*overwrite*/ virtual automachine::machine_meta* operator()(automachine::machine_meta*const* metas, int C, automachine::machine_meta* result)
    {
        interlanguage& iml = compiler::getiml();
        scope* s = iml.current_scope();
        const lalr1meta* m0 = (const lalr1meta*)(metas[0]);
        lalr1meta* ot = (lalr1meta*)(result); 

        const type* canTypes[] =
        {
            typesystem::instance().int_type(),
            typesystem::instance().float_type()
        };

        variable* v = s->entry_value(as<word>(m0->content)->txt, canTypes, 2);
		if (NULL == v) fire("invalidate value " + as<word>(m0->content)->txt);
		v->more() = (void*)".LC1";
        ot->content = v;
        ot->ctype = v->vtype();

        return result;
    }
};

// FuncCall -> symbol = symbol ( symbol , symbol )
struct prod_func15 : public ifunction
{
    /*overwrite*/ virtual automachine::machine_meta* operator()(automachine::machine_meta*const* metas, int C, automachine::machine_meta* result)
    {
        interlanguage& iml = compiler::getiml();
        scope* s = iml.current_scope();
        const lalr1meta* m0 = (const lalr1meta*)(metas[0]);
        const lalr1meta* m2 = (const lalr1meta*)(metas[2]);
        const lalr1meta* m4 = (const lalr1meta*)(metas[4]);
        const lalr1meta* m6 = (const lalr1meta*)(metas[6]);
        lalr1meta* ot = (lalr1meta*)(result);

		// find given function
		//variable* fun = s->find(as<word>(m2->content)->txt);
		//if (fun == NULL || !is<function_type>(fun->type))
		//	fire("undefined function name: " + as<word>(m2->content)->txt);

		variable* dst = s->find(as<word>(m0->content)->txt);
		if (dst == NULL) fire("undefined symbol " + as<word>(m0->content)->txt);
		variable* fun = s->find(as<word>(m2->content)->txt);
		if (fun == NULL) fire("undefined symbol " + as<word>(m2->content)->txt);
		else if(!is<function_type>(fun->vtype())) fire("undefined function name " + as<word>(m2->content)->txt);
		function_type* ft = (function_type*)as<function_type>(fun->vtype());
		variable* v1 = s->find(as<word>(m4->content)->txt);
		if (v1 == NULL) fire("undefined symbol " + as<word>(m4->content)->txt);
		else if (ft->params_type.size() <= 0 || ft->params_type[0] != v1->vtype())
			fire("function call(%s) param1(%s)'s is not correct", fun->name().c_str(), as<word>(m4->content)->txt.c_str());
		variable* v2 = s->find(as<word>(m6->content)->txt);
		if (v2 == NULL) fire("undefined symbol " + as<word>(m6->content)->txt);
		else if (ft->params_type.size() <= 1 || ft->params_type[1] != v1->vtype())
			fire("function call(%s) param1(%s)'s is not correct", fun->name().c_str(), as<word>(m6->content)->txt.c_str());
        std::auto_ptr<funcparamlist> funcparams(new funcparamlist(2, v1, v2));
        operation call_op(operations::func_call);
        s->entry_tuple(&call_op, fun, funcparams.release(), dst);
        
        ot->content = m0->content;
        ot->ctype = m0->ctype;
        return result;
    }
};

// PrintFunc -> print ( AValue )
struct prod_func16 : public ifunction
{
    /*overwrite*/ virtual automachine::machine_meta* operator()(automachine::machine_meta*const* metas, int C, automachine::machine_meta* result)
    {
        interlanguage& iml = compiler::getiml();
        scope* s = iml.current_scope();
        const lalr1meta* m0 = (const lalr1meta*)(metas[0]);
        const lalr1meta* m2 = (const lalr1meta*)(metas[2]);
        lalr1meta* ot = (lalr1meta*)(result);

        operation print_op(operations::internal);
		// m0->content is not a variable
		//internal_call* icall = new internal_call(as<word>(m0->content)->txt);
        s->entry_tuple(&print_op, NULL, m2->content, NULL);
        
        return result;
    }
};

// Function -> FunctionBegin FunctionContent ReturnExp }
struct prod_func17 : public ifunction
{
    /*overwrite*/ virtual automachine::machine_meta* operator()(automachine::machine_meta*const* metas, int C, automachine::machine_meta* result)
    {
        interlanguage& iml = compiler::getiml();
        scope* s = iml.current_scope();
        lalr1meta* ot = (lalr1meta*)(result);

        iml.otof_scope();
        return result;
    }
};

// ReturnExp -> return AValue ;
struct prod_func20 : public ifunction
{
    /*overwrite*/ virtual automachine::machine_meta* operator()(automachine::machine_meta*const* metas, int C, automachine::machine_meta* result)
    {
        interlanguage& iml = compiler::getiml();
        scope* s = iml.current_scope();
        const lalr1meta* m1 = (const lalr1meta*)(metas[1]);
        lalr1meta* ot = (lalr1meta*)(result);

        operation ret_op(operations::func_ret);
        s->entry_tuple(&ret_op, m1->content, NULL, NULL);
        return result;
    }
};

// Valuetype -> int
struct prod_func21 : public ifunction
{
    /*overwrite*/ virtual automachine::machine_meta* operator()(automachine::machine_meta*const* metas, int C, automachine::machine_meta* result)
    {
        interlanguage& iml = compiler::getiml();
        scope* s = iml.current_scope();
        const lalr1meta* m0 = (const lalr1meta*)(metas[0]);
        lalr1meta* ot = (lalr1meta*)(result);
        ot->content = typesystem::instance().int_type();
        ot->ctype = typesystem::instance().type_type(); // type_type
        return result;
    }
};

// Valuetype -> float
struct prod_func22 : public ifunction
{
    /*overwrite*/ virtual automachine::machine_meta* operator()(automachine::machine_meta*const* metas, int C, automachine::machine_meta* result)
    {
        interlanguage& iml = compiler::getiml();
        scope* s = iml.current_scope();
        const lalr1meta* m0 = (const lalr1meta*)(metas[0]);
        lalr1meta* ot = (lalr1meta*)(result);
        ot->content = typesystem::instance().float_type();
        ot->ctype = typesystem::instance().type_type(); // type_type
        return result;
    }
};

// Op -> +
struct prod_func23 : public ifunction
{
    /*overwrite*/ virtual automachine::machine_meta* operator()(automachine::machine_meta*const* metas, int C, automachine::machine_meta* result)
    {
        interlanguage& iml = compiler::getiml();
        scope* s = iml.current_scope();
        const lalr1meta* m0 = (const lalr1meta*)(metas[0]);
        lalr1meta* ot = (lalr1meta*)(result);
        operation* op = new operation(operations::plus);
        ot->content = op;
        ot->ctype = typesystem::instance().operator_type(); // operator_type
        return result;
    }
};

// Op -> -
struct prod_func24 : public ifunction
{
    /*overwrite*/ virtual automachine::machine_meta* operator()(automachine::machine_meta*const* metas, int C, automachine::machine_meta* result)
    {
        interlanguage& iml = compiler::getiml();
        scope* s = iml.current_scope();
        const lalr1meta* m0 = (const lalr1meta*)(metas[0]);
        lalr1meta* ot = (lalr1meta*)(result);
        operation* op = new operation(operations::minus);
        ot->content = op;
        ot->ctype = typesystem::instance().operator_type(); // operator_type
        return result;
    }
};

// Op -> *
struct prod_func25 : public ifunction
{
    /*overwrite*/ virtual automachine::machine_meta* operator()(automachine::machine_meta*const* metas, int C, automachine::machine_meta* result)
    {
        interlanguage& iml = compiler::getiml();
        scope* s = iml.current_scope();
        const lalr1meta* m0 = (const lalr1meta*)(metas[0]);
        lalr1meta* ot = (lalr1meta*)(result);
        operation* op = new operation(operations::multi);
        ot->content = op;
        ot->ctype = typesystem::instance().operator_type(); // operator_type
        return result;
    }
};

// Op -> /
struct prod_func26 : public ifunction
{
    /*overwrite*/ virtual automachine::machine_meta* operator()(automachine::machine_meta*const* metas, int C, automachine::machine_meta* result)
    {
        interlanguage& iml = compiler::getiml();
        scope* s = iml.current_scope();
        const lalr1meta* m0 = (const lalr1meta*)(metas[0]);
        lalr1meta* ot = (lalr1meta*)(result);
        operation* op = new operation(operations::divid);
        ot->content = op;
        ot->ctype = typesystem::instance().operator_type(); // operator_type
        return result;
    }
};

// FunctionHeader -> ValueType symbol ( ValueType symbol , ValueType symbol )
struct prod_func27 : public ifunction
{
    /*overwrite*/ virtual automachine::machine_meta* operator()(automachine::machine_meta*const* metas, int C, automachine::machine_meta* result)
    {
        interlanguage& iml = compiler::getiml();
        scope* s = iml.current_scope();
        const lalr1meta* m0 = (const lalr1meta*)(metas[0]);
        const lalr1meta* m1 = (const lalr1meta*)(metas[1]);
        const lalr1meta* m3 = (const lalr1meta*)(metas[3]);
        const lalr1meta* m4 = (const lalr1meta*)(metas[4]);
        const lalr1meta* m6 = (const lalr1meta*)(metas[6]);
        const lalr1meta* m7 = (const lalr1meta*)(metas[7]);
        lalr1meta* ot = (lalr1meta*)(result);

		// function header
		const type* ts[2] = 
		{
			as<type>(m3->content), as<type>(m6->content)
		};
		function_type* ftype = (function_type*)typesystem::instance().new_func_type(2, ts, as<type>(m0->content));
		variable* var = new variable(as<word>(m1->content)->txt, ftype, NULL, NULL);
		kog::smart_vector<_Str>* pv = new kog::smart_vector<_Str>(2);
		(*pv)[0] = as<word>(m4->content)->txt;
		(*pv)[1] = as<word>(m7->content)->txt;
		ftype->more = pv;

        ot->content = var;
        ot->ctype = ftype;

        return result;
    }
};

// FunctionBegin -> FunctionHeader {
struct prod_func28 : public ifunction
{
    /*overwrite*/ virtual automachine::machine_meta* operator()(automachine::machine_meta*const* metas, int C, automachine::machine_meta* result)
    {
        interlanguage& iml = compiler::getiml();
        scope* s = iml.current_scope();
        const lalr1meta* m0 = (const lalr1meta*)(metas[0]);
        lalr1meta* ot = (lalr1meta*)(result);

		variable* fvar = (variable*)as<variable>(m0->content);
		function_type* ft = (function_type*)fvar->vtype();
		funcscope* ns = new funcscope(s, ft);
		ns->name() = fvar->name();
		delete fvar;
		iml.push_scope(ns);

		fvar = s->entry_function(0, ns->name(), ft);
		fvar->more() = (void*)ns->name().c_str();

		kog::smart_vector<_Str>* pv = (kog::smart_vector<_Str>*)ft->more;

		// entry params
		for(size_t i = 0; i < ft->params_type.size(); ++ i)
		{
			variable* v = ns->entry_variable((*pv)[i], ft->params_type[i], varscope::vparam);
			v->more() = (void*)"-4(%ebp)";
		}
		((variable*)ns->return_var())->more() = (void*)"4(%ebp)";

		ot->content = m0->content;
		ot->ctype = m0->ctype;

		return result;
	}
};

void init_production_functions(kog::smart_vector<ifunction*>& pfuncs)
{
    pfuncs.reset(29);
    pfuncs[0] = new prod_func_default();
    pfuncs[1] = new prod_func_default();
    pfuncs[2] = new prod_func_default();
    pfuncs[3] = new prod_func_default();
    pfuncs[4] = new prod_func4();
    pfuncs[5] = new prod_func_default();
    pfuncs[6] = new prod_func_default();
    pfuncs[7] = new prod_func_default();
    pfuncs[8] = new prod_func_default();
    pfuncs[9] = new prod_func_default();
    pfuncs[10] = new prod_func_default();
    pfuncs[11] = new prod_func_default();
    pfuncs[12] = new prod_func12();
    pfuncs[13] = new prod_func13();
    pfuncs[14] = new prod_func14();
    pfuncs[15] = new prod_func15();
    pfuncs[16] = new prod_func16();
    pfuncs[17] = new prod_func17();
    pfuncs[18] = new prod_func_default();
    pfuncs[19] = new prod_func_default();
    pfuncs[20] = new prod_func20();
    pfuncs[21] = new prod_func21();
    pfuncs[22] = new prod_func22();
    pfuncs[23] = new prod_func23();
    pfuncs[24] = new prod_func24();
    pfuncs[25] = new prod_func25();
    pfuncs[26] = new prod_func26();
    pfuncs[27] = new prod_func27();
    pfuncs[28] = new prod_func28();
}
