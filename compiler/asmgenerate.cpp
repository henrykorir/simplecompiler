#include "asmgenerate.h"
#include <map>

using namespace compile;
using namespace compile::runtime;

asmgenerate::asmgenerate(std::ostream& os)
    : os_(&os)
{
}

void asmgenerate::print(scope* s)
{
    std::map<object*, _Str> varpos; 
    std::ostream& os = *os_;
    for (std::deque<tuple*>::const_iterator iter = s->code()->tuples().begin(); iter != s->code()->tuples().end(); ++ iter)
    {
        using namespace operations;
        four_tuple* pt = (four_tuple*)(*iter);
        size_t esp = 0;
        switch (pt->oper)
        {
        case newv:
            os<<"addl $"<<pt->src1->vtype()->tsize<<", %%esp"<<std::endl;
            varpos[pt->src1] = stringX::format("%d(%%ebp)", esp);
            esp += pt->src1->vtype()->tsize;
            break;
        case plus:
            if (pt->src1->vtype() != pt->src2->vtype())
                fire("invalidate type when add: %s %s", pt->src1->vtype()->to_string().c_str(), pt->src2->vtype()->to_string().c_str());
            else if(pt->src1->vtype() == typesystem::instance().int_type())
            {
                os<<"movl "<<varpos[pt->src1]<<", %%eax"<<std::endl;
                os<<"movl "<<varpos[pt->src2]<<", %%ebx"<<std::endl;
                os<<"addl %%eax, %%ebx"<<std::endl;
                os<<"movl %%ebx, "<<varpos[pt->dst]<<std::endl;
            }
            else if(pt->src2->vtype() == typesystem::instance().float_type())
            {
                os<<"movl "<<varpos[pt->src1]<<", %%eax"<<std::endl;
                os<<"movl "<<varpos[pt->src2]<<", %%ebx"<<std::endl;
                os<<"addf %%eax, %%ebx"<<std::endl;
                os<<"movl %%ebx, "<<varpos[pt->dst]<<std::endl;
            }
            else fire("invalidate type!");
            break;
        case minus:
            if (pt->src1->vtype() != pt->src2->vtype())
                fire("invalidate type when add: %s %s", pt->src1->vtype()->to_string().c_str(), pt->src2->vtype()->to_string().c_str());
            else if(pt->src1->vtype() == typesystem::instance().int_type())
            {
                os<<"movl "<<varpos[pt->src1]<<", %%eax"<<std::endl;
                os<<"movl "<<varpos[pt->src2]<<", %%ebx"<<std::endl;
                os<<"minusl %%eax, %%ebx"<<std::endl;
                os<<"movl %%ebx, "<<varpos[pt->dst]<<std::endl;
            }
            else if(pt->src2->vtype() == typesystem::instance().float_type())
            {
                os<<"movl "<<varpos[pt->src1]<<", %%eax"<<std::endl;
                os<<"movl "<<varpos[pt->src2]<<", %%ebx"<<std::endl;
                os<<"minusf %%eax, %%ebx"<<std::endl;
                os<<"movl %%ebx, "<<varpos[pt->dst]<<std::endl;
            }
            else fire("invalidate type!");
            break;
        case func_call:
            break;
        case func_ret:
            break;
        case internal:
            break;
        default:
            fire("invalidate operator: %s", operation(pt->oper).to_string().c_str());
        }
    }
}

