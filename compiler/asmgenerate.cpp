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
	os<<"\t.file \""<<m->name()<<"\"";

	print_printasm();
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
	//foreach (const session_map& si, ps->sessions().begin(), ps->sessions().end())
	scope::session_map::const_iterator iter = ps->sessions().find(typeid(rodata_session).name());
	if (iter != ps->sessions().end())
	{
		session* ss = iter->second;
		_Str sname = ss->name();//iter->first;
		logstring("print section[%s]", sname.c_str());

		os<<newline<<".section .rodata"<<inctab;
		const std::deque<variable*>& vars = as<rodata_session>(ss)->variables();
		foreach (const variable* v, vars.begin(), vars.end())
		{
			os<<newline<<(const char*)v->more()<<":"<<inctab
				<<newline<<".byte "<<byte2str(v->initVar()->initv, v->initVar()->size)<<dectab;
		}
		os<<dectab;
	}

	iter = ps->sessions().find(typeid(text_session).name());
	if (iter != ps->sessions().end())
	{
		session* ss = iter->second;
		_Str sname = ss->name();//iter->first;
		logstring("print session[%s]", sname.c_str());
		print_code(ps->name(), as<text_session>(ss));
	}
	iter = ps->sessions().find(typeid(data_session).name());
	if (iter != ps->sessions().end())
	{
		session* ss = iter->second;
		_Str sname = ss->name();//iter->first;
		logstring("print session[%s]", sname.c_str());
	//  os<<newline<<".section .data"<<inctab;
	//	const std::deque<variable*>& vars = as<data_session>(si.second)->variables();
	//	foreach (const variable* v, vars.begin(), vars.end())
	//	{
	//		os<<newline<<(const char*)v->more()<<inctab
	//			<<newline<<".byte "<<varpos(v)<<dectab;
	//	}
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

	static int lfb_i = 2;
	logstring("asm.print code");
	//os<<newline<<".section .code"
	os<<inctab<<newline<<".text"
		<<newline<<".global "<<name
		<<newline<<".type "<<name<<", @function"<<dectab
		<<newline<<name<<":"
		<<newline<<".LFB"<<lfb_i++<<":"<<inctab
		<<newline<<"pushl %ebp"
		<<newline<<"movl %esp, %ebp"
		<<newline<<"subl $"<<ts->env()->bytes()<<", %esp";
	bool is_init_float = false;
    for (std::deque<tuple*>::const_iterator iter = ts->tuples().begin(); iter != ts->tuples().end(); ++ iter)
    {
        using namespace operations;
        four_tuple* pt = (four_tuple*)(*iter);
        switch (pt->oper)
        {
		case assign:
			logstring("asm.print code: assign");
			if (pt->src1->vtype()->tsize != 4 || pt->dst->vtype()->tsize != 4)
				fire("only support size 4 assign");
			if (((const char*)pt->src1->more())[0] == '.')
			{
				os<<newline<<"movl $"<<varpos(pt->src1)<<", %eax"
					<<newline<<"movl (%eax), %ebx"
					<<newline<<"movl %ebx, "<<varpos(pt->dst);
			}
			else
			{
				os<<newline<<"movl "<<varpos(pt->src1)<<", "<<varpos(pt->dst);
			}
			break;
        case plus:
			logstring("asm.print code: plus");
            if (pt->src1->vtype() != pt->src2->vtype())
                fire("invalidate type when add: %s %s", pt->src1->vtype()->to_string().c_str(), pt->src2->vtype()->to_string().c_str());
            else if(pt->src1->vtype() == typesystem::instance().get_type("int"))
            {
                os<<newline<<"movl "<<varpos(pt->src1)<<", %eax";
                os<<newline<<"movl "<<varpos(pt->src2)<<", %ebx";
                os<<newline<<"addl %eax, %ebx";
                os<<newline<<"movl %ebx, "<<varpos(pt->dst);
            }
            else if(pt->src2->vtype() == typesystem::instance().get_type("float"))
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
            else if(pt->src1->vtype() == typesystem::instance().get_type("int"))
            {
                os<<newline<<"movl "<<varpos(pt->src1)<<", %eax";
                os<<newline<<"movl "<<varpos(pt->src2)<<", %ebx";
                os<<newline<<"subl %eax, %ebx";
                os<<newline<<"movl %ebx, "<<varpos(pt->dst);
            }
            else if(pt->src2->vtype() == typesystem::instance().get_type("float"))
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
            else if(pt->src1->vtype() == typesystem::instance().get_type("int"))
            {
                os<<newline<<"movl "<<varpos(pt->src1)<<", %eax";
                os<<newline<<"movl "<<varpos(pt->src2)<<", %ebx";
                os<<newline<<"imul %eax, %ebx";
                os<<newline<<"movl %ebx, "<<varpos(pt->dst);
            }
            else if(pt->src2->vtype() == typesystem::instance().get_type("float"))
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
            else if(pt->src1->vtype() == typesystem::instance().get_type("int"))
            {
                os<<newline<<"movl "<<varpos(pt->src1)<<", %eax";
                os<<newline<<"movl "<<varpos(pt->src2)<<", %ebx";
                os<<newline<<"divl %eax, %ebx";
                os<<newline<<"movl %ebx, "<<varpos(pt->dst);
            }
            else if(pt->src2->vtype() == typesystem::instance().get_type("float"))
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
					<<newline<<"addl $"<<ssize<<", %esp"; // pop params and return value's address
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
					<<newline<<"movl "<<varpos(return_value)<<", %ebx"
					<<newline<<"movl %eax, (%ebx)" // move to return value
					<<newline<<"leave"
                	<<newline<<"ret";
            }
            break;
		case internal:
			{
				logstring("asm.print code: internal");
				// internal_call* icall = as<internal_call>(src1);
				// print 
				if (pt->src2->vtype() == typesystem::instance().get_type("int"))
				{
					os<<newline<<"pushl "<<varpos(pt->src2)
						<<newline<<"call print_int";
				}
				else if (pt->src2->vtype() == typesystem::instance().get_type("float"))
				{
					os<<newline<<"pushl "<<varpos(pt->src2)
						<<newline<<"call print_float";
				}
				os<<newline<<"addl $"<<pt->src2->vtype()->tsize<<", %esp";
			}
			break;
        default:
            fire("invalidate operator: %s", operation(pt->oper).to_string().c_str());
        }
		logstring("finish asm.print an operation");
	}
	os<<dectab;
	logstring("finish asm.print scope(%s)'s code", name.c_str());
}

void asmgenerate::print_printasm()
{
    kog::oindentstream os(*os_); 
    using kog::inctab;
    using kog::dectab;
    using kog::newline;

	// print_int
	os<<inctab<<newline<<".section .rodata"<<dectab
		<<newline<<".LC_kog0:"<<inctab
		<<newline<<".string \"%d\\n\""
		<<newline<<".text "
		<<newline<<".global print_int"
		<<newline<<".type print_int, @function"<<dectab
		<<newline<<"print_int:"
		<<newline<<".LFB0:"<<inctab
		<<newline<<"pushl %ebp"
		<<newline<<"movl %esp, %ebp"
		<<newline<<"movl 8(%ebp), %eax"
		<<newline<<"pushl %eax"
		<<newline<<"movl $.LC_kog0, %eax"
		<<newline<<"pushl %eax"
		<<newline<<"call printf"
		<<newline<<"leave"
		<<newline<<"ret"<<dectab
		<<std::endl;

	os<<inctab<<newline<<".section .rodata"<<dectab
		<<newline<<".LC_kog1:"<<inctab
		<<newline<<".string \"%f\\n\""
		<<newline<<".text "
		<<newline<<".global print_float"
		<<newline<<".type print_float, @function"<<dectab
		<<newline<<"print_float:"
		<<newline<<".LFB1:"<<inctab
		<<newline<<"pushl %ebp"
		<<newline<<"movl %esp, %ebp"
		<<newline<<"subl $12, %esp"
		<<newline<<"flds 8(%ebp)"
		<<newline<<"fstpl -8(%ebp)"
		<<newline<<"movl $.LC_kog1, %eax"
		<<newline<<"movl %eax, -12(%ebp)"
		<<newline<<"call printf"
		<<newline<<"leave"
		<<newline<<"ret"<<dectab
		<<std::endl;
}
