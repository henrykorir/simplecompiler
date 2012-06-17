#include <ifunction.h>
#include <interlanguage.h>
#include <compiler.h>
#include <funcparams.h>

using namespace compile;
using namespace compile::doc;
using namespace compile::runtime;

typedef lalr1machine::lalr1meta lalr1meta;

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
        ot->content = s->entry_variable(as<word>(m1->content)->txt, vt); // new variable
        ot->ctype = vt;

        return result;
    }
};

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

        s-> entry_tuple(as<operation>(m3->content), m2->content, m4->content, m0->content); // op, src1, src2, dst
        ot->content = m0->content;
        ot->ctype = m0->ctype;
        return result;
    }
};

struct prod_func13 : public ifunction
{
    /*overwrite*/ virtual automachine::machine_meta* operator()(automachine::machine_meta*const* metas, int C, automachine::machine_meta* result)
    {
        interlanguage& iml = compiler::getiml();
        scope* s = iml.current_scope();
        const lalr1meta* m0 = (const lalr1meta*)(metas[0]);
        lalr1meta* ot = (lalr1meta*)(result);

        variable* v = s->find(as<word>(m0->content)->txt);
        ot->content = v;
        ot->ctype = v->vtype();

        return result;
    }
};

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
        ot->content = v;
        ot->ctype = v->vtype();

        return result;
    }
};

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

        funcparamlist* funcparams = new funcparamlist(2, m4->content, m6->content);
        operation* call_op = new operation(operations::func_call);
        s->entry_tuple(call_op, m2->content, funcparams, m0->content);
        
        ot->content = m0->content;
        ot->ctype = m0->ctype;
        return result;
    }
};

struct prod_func16 : public ifunction
{
    /*overwrite*/ virtual automachine::machine_meta* operator()(automachine::machine_meta*const* metas, int C, automachine::machine_meta* result)
    {
        interlanguage& iml = compiler::getiml();
        scope* s = iml.current_scope();
        const lalr1meta* m0 = (const lalr1meta*)(metas[0]);
        const lalr1meta* m2 = (const lalr1meta*)(metas[2]);
        lalr1meta* ot = (lalr1meta*)(result);

        operation* print_op = new operation(operations::internal);
        s->entry_tuple(print_op, m0->content, m2->content, NULL);
        
        return result;
    }
};

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

struct prod_func20 : public ifunction
{
    /*overwrite*/ virtual automachine::machine_meta* operator()(automachine::machine_meta*const* metas, int C, automachine::machine_meta* result)
    {
        interlanguage& iml = compiler::getiml();
        scope* s = iml.current_scope();
        const lalr1meta* m1 = (const lalr1meta*)(metas[1]);
        lalr1meta* ot = (lalr1meta*)(result);

        operation* ret_op = new operation(operations::func_ret);
        s->entry_tuple(ret_op, m1->content, NULL, NULL);
        return result;
    }
};

struct prod_func21 : public ifunction
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

struct prod_func22 : public ifunction
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

        variable* f = s->entry_function(as<word>(m1->content)->txt, as<type>(m0->content), 2, as<type>(m3->content), as<type>(m6->content));
        scope* ns = iml.new_scope();
        
        variable* v1 = ns->entry_variable(as<word>(m4->content)->txt, as<type>(m3->content));
        variable* v2 = ns->entry_variable(as<word>(m7->content)->txt, as<type>(m6->content));

        ot->content = f;
        ot->ctype = typesystem::instance().functiontype_type();

        return result;
    }
};

void init_production_functions(kog::smart_vector<ifunction*>& pfuncs)
{
    pfuncs.reset(28);
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
}
