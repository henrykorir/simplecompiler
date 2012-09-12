#include "asmgenerate.h"
#include <map>
#include <indentstream.h>
#include <sstream>
#include <stack>
#include "funcparams.h"
#include <logger.h>

using namespace compile;
using namespace compile::runtime;

asmgenerate::asmgenerate(std::ostream& os)
    : os_(&os)
{
}

void asmgenerate::print(module* m)
{
	logstring("asm.print(module)");
    kog::oindentstream os(*os_); 
	os<<kog::newline<<".file "<<m->name();

	// build refence graph
	// not do now
	
	//std::vector<scope*> otscopes;
	std::stack<std::pair<scope*, bool> > ptStack;
	ptStack.push(std::make_pair(m->root_scope(), false));
	while (!ptStack.empty())
	{
		scope* ps = ptStack.top().first;
		if (ps == NULL) fire("unknown error, scope is null!");
		logstring("check scope " + ps->name());
		if (ptStack.top().second)
		{
			// print asm
			print(ps);
			ptStack.pop();
		}
		else
		{
			ptStack.top().second = true;
			foreach (scope* p, ps->children().begin(), ps->children().end())
			{
				if (is<funcscope>(p) || is<slicescope>(p))
				{
					logstring("print stack, add scope " + p->name());
					ptStack.push(std::make_pair(p, false));
				}
			}
		}
	}
}

_Str byte2str(byte* buf, uint32 size)
{
	std::ostringstream oss;
	const char* sep = "";
	for (uint32 i = 0; i < size; ++ i, sep = ", ")
	{
		oss<<sep<<"0x"<<std::hex<<(unsigned int)(buf[i]);
	}
	return oss.str();
}

void asmgenerate::print(scope* ps)
{
	logstring("asm.print scope: " + ps->name());
    kog::oindentstream os(*os_); 
    using kog::inctab;
    using kog::dectab;
    using kog::newline;
	typedef std::pair<_Str, session*> session_map;
	logstring("count of sessions is %d", ps->sessions().size());
	size_t ii = 0;
	//foreach (const session_map& si, ps->sessions().begin(), ps->sessions().end())
	for (scope::session_map::const_iterator iter = ps->sessions().begin(); iter != ps->sessions().end(); ++ iter)
	//while(ii == 0)
	{
		//if (++ii > ps->sessions().size()) break;
		session* ss = iter->second;
		logstring("ss%s", typeid(*ss).name());
		_Str sname = ss->name();//iter->first;
		//const session_map& si = *iter;
		logstring("print session[%s]", sname.c_str());
		if (sname == typeid(rodata_session).name())
		{
			//os<<newline<<".session .rodata"<<inctab;

			//const std::deque<variable*>& vars = as<rodata_session>(si.second)->variables();
			//foreach (const variable* v, vars.begin(), vars.end())
			//{
			//	os<<newline<<(const char*)v->more()<<":"<<inctab
			//		<<newline<<".byte "<<byte2str(v->initVar()->initv, v->initVar()->size)<<dectab;
			//}
		}
		else if(sname == typeid(data_session).name())
		{
    	//    os<<newline<<".section .data"<<inctab;
		//	const std::deque<variable*>& vars = as<data_session>(si.second)->variables();
		//	foreach (const variable* v, vars.begin(), vars.end())
		//	{
		//		os<<newline<<(const char*)v->more()<<inctab
		//			<<newline<<".byte "<<varpos(v)<<dectab;
		//	}
		}
		else if(sname == typeid(text_session).name())
		{
			//print_code(ps->name(), as<text_session>(si.second));
			print_code(ps->name(), as<text_session>(ss));
		}
		else
		{
			logstring("unknown session");
		}
	}

	os<<newline;
}

const char* varpos(const variable* v)
{
	return (const char*)v->more();
}

void asmgenerate::print_code(const _Str& name, text_session* ts)
{
    kog::oindentstream os(*os_); 
    using kog::inctab;
    using kog::dectab;
    using kog::newline;

	logstring("asm.print code");
	//os<<newline<<".section .code"
	os<<newline<<".text"
		<<newline<<".global "<<name
		<<newline<<".type "<<name<<", @function"
		<<newline<<name<<":"<<inctab
		<<newline<<"pushl %ebp"
		<<newline<<"movl %esp, %ebp";
	bool is_init_float = false;
    for (std::deque<tuple*>::const_iterator iter = ts->tuples().begin(); iter != ts->tuples().end(); ++ iter)
    {
        using namespace operations;
        four_tuple* pt = (four_tuple*)(*iter);
        switch (pt->oper)
        {
        case plus:
			logstring("asm.print code: plus");
            if (pt->src1->vtype() != pt->src2->vtype())
                fire("invalidate type when add: %s %s", pt->src1->vtype()->to_string().c_str(), pt->src2->vtype()->to_string().c_str());
            else if(pt->src1->vtype() == typesystem::instance().int_type())
            {
                os<<newline<<"movl "<<varpos(pt->src1)<<", %eax";
                os<<newline<<"movl "<<varpos(pt->src2)<<", %ebx";
                os<<newline<<"addl %eax, %ebx";
                os<<newline<<"movl %ebx, "<<varpos(pt->dst);
            }
            else if(pt->src2->vtype() == typesystem::instance().float_type())
            {
                os<<newline<<"flds "<<varpos(pt->src1);
                os<<newline<<"fadds "<<varpos(pt->src2);
                os<<newline<<"fstps "<<varpos(pt->dst);
            }
            else fire("invalidate type!");
            break;
        case minus:
			logstring("asm.print code: minus");
            if (pt->src1->vtype() != pt->src2->vtype())
                fire("invalidate type when add: %s %s", pt->src1->vtype()->to_string().c_str(), pt->src2->vtype()->to_string().c_str());
            else if(pt->src1->vtype() == typesystem::instance().int_type())
            {
                os<<newline<<"movl "<<varpos(pt->src1)<<", %eax";
                os<<newline<<"movl "<<varpos(pt->src2)<<", %ebx";
                os<<newline<<"subl %eax, %ebx";
                os<<newline<<"movl %ebx, "<<varpos(pt->dst);
            }
            else if(pt->src2->vtype() == typesystem::instance().float_type())
            {
                os<<newline<<"flds "<<varpos(pt->src1);
                os<<newline<<"fsubs "<<varpos(pt->src2);
                os<<newline<<"fstps "<<varpos(pt->dst);
            }
            else fire("invalidate type!");
            break;
        case multi:
			logstring("asm.print code: multi");
            if (pt->src1->vtype() != pt->src2->vtype())
                fire("invalidate type when add: %s %s", pt->src1->vtype()->to_string().c_str(), pt->src2->vtype()->to_string().c_str());
            else if(pt->src1->vtype() == typesystem::instance().int_type())
            {
                os<<newline<<"movl "<<varpos(pt->src1)<<", %eax";
                os<<newline<<"movl "<<varpos(pt->src2)<<", %ebx";
                os<<newline<<"mull %eax, %ebx";
                os<<newline<<"movl %ebx, "<<varpos(pt->dst);
            }
            else if(pt->src2->vtype() == typesystem::instance().float_type())
            {
                os<<newline<<"flds "<<varpos(pt->src1);
                os<<newline<<"fmuls "<<varpos(pt->src2);
                os<<newline<<"fstps "<<varpos(pt->dst);
            }
            else fire("invalidate type!");
            break;
        case divid:
			logstring("asm.print code: divid");
            if (pt->src1->vtype() != pt->src2->vtype())
                fire("invalidate type when add: %s %s", pt->src1->vtype()->to_string().c_str(), pt->src2->vtype()->to_string().c_str());
            else if(pt->src1->vtype() == typesystem::instance().int_type())
            {
                os<<newline<<"movl "<<varpos(pt->src1)<<", %eax";
                os<<newline<<"movl "<<varpos(pt->src2)<<", %ebx";
                os<<newline<<"divl %eax, %ebx";
                os<<newline<<"movl %ebx, "<<varpos(pt->dst);
            }
            else if(pt->src2->vtype() == typesystem::instance().float_type())
            {
                os<<newline<<"flds "<<varpos(pt->src1);
                os<<newline<<"fdivs "<<varpos(pt->src2);
                os<<newline<<"fstps "<<varpos(pt->dst);
            }
            else fire("invalidate type!");
            break;
        case func_call:
			{
				logstring("asm.print code: func_call");
				// src1 content --> function type
				// 	push all params: src2 is funcparam list
				//  if dst is not null, push dst's address in stack
				//  call src1->function_re_name

				size_t ssize = 0;
				funcparamlist* plist = as<funcparamlist>(pt->src2);
				for (size_t i = plist->params_.size(); i; -- i)
				{
					// push plist->params[i-1]
					os<<newline<<"pushl "<<varpos(as<variable>(plist->params_[i - 1]));
					ssize += as<variable>(plist->params_[i-1])->vtype()->tsize;
				}
				
				if (pt->dst != NULL) 
				{
					os<<newline<<"lea "<<varpos(pt->dst)<<", %eax"
						<<newline<<"pushl %eax";
					ssize += pt->dst->vtype()->tsize;
				}

				// os<<newline<<"call "<<src1->name();
				// here src1->more: function name
				os<<newline<<"call "<<varpos(pt->src1)
					<<newline<<"subl $"<<ssize<<", %esp"; // pop params and return value's address
			}

            break;
        case func_ret:
			logstring("asm.print code: func_ret");
            if (pt->src1 != NULL) // non-null
            {
				funcscope* fs = as<funcscope>(ts->env());
				// if has return value
				function_type* ft = fs->ftype();
				if (pt->src1->vtype() != ft->return_type)
					fire("except return type is %d, but return value is %d",
							pt->src1->vtype()->tid, ft->return_type == NULL ? -1 : ft->return_type->tid);
				variable* return_value = fs->return_var();
				// using given funcscope, to find detect return value is void?
				//  is not void, movl return value to given address
                os<<newline<<"movl "<<varpos(pt->src1)<<", %eax"
					<<newline<<"lea "<<varpos(return_value)<<", %ebx"
					<<newline<<"movl %eax (%ebx)" // move to return value
					<<newline<<"leave"
                	<<newline<<"ret";
            }
            break;
		case internal:
			{
				logstring("asm.print code: internal");
				// internal_call* icall = as<internal_call>(src1);
				// print 
				if (pt->src2->vtype() == typesystem::instance().int_type())
				{
					os<<newline<<"pushl "<<varpos(pt->src2)
						<<newline<<"call print_int";
				}
				else if (pt->src2->vtype() == typesystem::instance().float_type())
				{
					os<<newline<<"pushl "<<varpos(pt->src2)
						<<newline<<"call print_float";
				}
			}
			break;
        default:
            fire("invalidate operator: %s", operation(pt->oper).to_string().c_str());
        }
		logstring("finish asm.print an operation");
	}
	os<<newline<<dectab
		<<std::endl;
	logstring("finish asm.print scope(%s)'s code", name.c_str());
}


