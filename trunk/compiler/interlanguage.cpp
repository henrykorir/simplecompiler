#include "interlanguage.h"
#include <functional>
#include <algorithm>
#include <logger.h>
#include <fstream>
#include <stack>
#include <scerror.h>
#include <indentstream.h>
#include <oformatstream.h>
#include <typeinfo>
#include <iomanip>

#include "extract.h"
#include "lalr1machine.h"
#include "asmgenerate.h"

using namespace sc;
using namespace compile;
using namespace compile::doc;
using namespace compile::runtime;

interlanguage::interlanguage()
: main_scope_(new slicescope())
{
	//main_scope_->name() = "global_scope";
	main_scope_->name() = "main";
    current_scope_ = main_scope_;
    all_scopes_.push_back(current_scope_);
	logstring("current scope(%s)", main_scope_->name().c_str());
}

interlanguage::~interlanguage()
{ 
	foreach (scope*& s, all_scopes_.begin(), all_scopes_.end())
    {
		//delete s;
		s = NULL;
    }
    all_scopes_.clear();
}

scope* interlanguage::push_scope(scope* s)
{
	logstring("push scope(%s)", s->name().c_str());
	s->parent() = current_scope_;
	current_scope_->children().push_back(s);
    all_scopes_.push_back(s);
    return current_scope_ = s;
}

scope* interlanguage::into_scope(runtime::scope* s)
{
    current_scope_ = s;
    return s;
}

scope* interlanguage::otof_scope()
{
    if (current_scope_ == NULL)
    {
        fire("root scope!");
    }
	logstring("out of scope(%s)", current_scope_->name().c_str());
    current_scope_ = current_scope_->parent();
	logstring("current scope is %s", current_scope_ == NULL ? "<null>" : current_scope_->name().c_str());
	return current_scope_;
}

scope* interlanguage::current_scope() const
{
    return current_scope_;
}

void interlanguage::generate(const std::string& finput_name, const std::string& fotput_name)
{
	module m(finput_name, main_scope_);
	std::ofstream ofile(fotput_name.c_str());
	if (!ofile) fire("can't open file: " + fotput_name);
	asmgenerate asmg(ofile);
	asmg.print(&m);
	//
	//foreach (scope* ps, all_scopes_.begin(), all_scopes_.end())
	//{
	//	asmg.print(ps);
	//}
	ofile.close();
}

void interlanguage::print_il(const std::string& ilfile) const
{
	std::ofstream ofs(ilfile.c_str());
	if (!ofs.is_open()) fire("can't open file(%s)", ilfile.c_str());
	kog::oformatstream os(ofs);
    //kog::oindentstream os(ofs);
	//os.setf(std::ostream::left);
	os<<std::left;
    using kog::inctab;
    using kog::dectab;
    using kog::fmtline;
	std::stack<scope*> ptStack;
	ptStack.push(main_scope_);
	while (!ptStack.empty())
	{
		scope* ps = ptStack.top();
		ptStack.pop();
		if (ps == NULL) continue;
		foreach (scope* p, ps->children().begin(), ps->children().end())
		{
			ptStack.push(p);
		}

		logstring("output scope(%s)", ps->name().c_str());
		os<<ps->name();

		os<<fmtline<<"========readonly data========"<<kog::flush<<inctab;
		const rodata_session* rods = as<rodata_session>(ps->get_session(typeid(rodata_session).name()));
		os<<fmtline<<"content"<<"type"<<"size";
		if (rods != NULL)
		{
			const std::deque<variable*>& vars = rods->variables();
			foreach (const variable* v, vars.begin(), vars.end())
			{
				os<<fmtline<<v->name()<<v->vtype()->to_string()<<v->vtype()->tsize;
			}
		}
		os<<kog::flush<<dectab<<kog::newline;
		
		os<<fmtline<<"========variables========"<<kog::flush<<inctab;
		// print scope's variables
		const data_session* ds = as<data_session>(ps->get_session(typeid(data_session).name()));
		os<<fmtline<<"name"<<"type"<<"size";
		if (ds != NULL)
		{
			const std::deque<variable*>& vars = ds->variables();
			foreach (const variable* v, vars.begin(), vars.end())
			{
				os<<fmtline<<v->to_string()<<v->vtype()->to_string()<<v->vtype()->tsize;
			}
		}
		os<<kog::flush<<dectab<<kog::newline;
		
		// print scope's tuples
		const text_session* ts = as<text_session>(ps->get_session(typeid(text_session).name()));
		if (ts == NULL) continue;

		os<<fmtline<<"========tuples========"<<kog::flush<<inctab
			<<fmtline<<"oper"<<"src1"<<"src2"<<"dest";
    	for (std::deque<tuple*>::const_iterator iter = ts->tuples().begin(); iter != ts->tuples().end(); ++ iter)
		{
       		four_tuple* pt = (four_tuple*)(*iter);
			os<<fmtline<<operations::op_string::to_string(pt->oper)<<obj2str(pt->src1)
				<<obj2str(pt->src2)<<obj2str(pt->dst);
		}

		os<<kog::flush<<dectab<<kog::newline<<kog::newline;
	}

	ofs.close();
}
