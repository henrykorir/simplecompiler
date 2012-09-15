#include "scope.h"
#include <cstdarg>
#include <memory>
#include <typeinfo>
#include <stringX.h>
#include <functionalX.h>
#include <logger.h>

using namespace compile;
using namespace compile::runtime;

scope::scope()
: parent_(NULL)
, noname_idx_(0)
, bytes_(0)
, stype_(varscope::sunknown)
{
}

scope::scope(scope* parent, int32 stype, bool isInherit)
: noname_idx_(0)
, parent_(parent)
, stype_(stype)
, bytes_(0)
{
	if (isInherit && NULL != parent_)
	{
		// inherit all using scopes from parent
		usingList_ = parent_->usingList_;
		// parent is also using
		usingList_.push_back(parent_);
	}
}

scope::~scope()
{
}

variable* scope::find_here(const _Str& name) const
{
	std::deque<variable*>* vars = NULL;
	if (name.size() > 4 && name.find("##") == 0 && name.rfind("##") == name.size() - 2)
	{
		session_map::const_iterator iter = sessions_.find(typeid(rodata_session).name());
		if (iter != sessions_.end())
		{
			rodata_session* rs = compile::as<rodata_session>(iter->second);
			vars = rs != NULL ? &(rs->variables()) : NULL;
		}
		else logstring("scope(%s) not found %s", name_.c_str(), typeid(rodata_session).name());
	}
	else
	{
		session_map::const_iterator iter = sessions_.find(typeid(data_session).name());
		if (iter != sessions_.end())
		{
			data_session* ds = compile::as<data_session>(iter->second);
			vars = ds != NULL ? &ds->variables() : NULL;
		}
		else logstring("scope(%s) not found %s", name_.c_str(), typeid(data_session).name());
	}
	if (vars == NULL) return NULL;

	foreach (variable* var, vars->begin(), vars->end())
	{
		if (var->name() == name)
			return var;
	}

	return NULL;
}

variable* scope::find(const _Str& name)
{
	logstring("try to find %s in scope(%s)", name.c_str(), this->name_.c_str());
	symboltable::const_iterator iter = reg_table_.find(name);
	if (iter != reg_table_.end())
	{
		return iter->second; 
	}
	// find it
	variable* p = NULL;
	if (NULL == (p = find_here(name)))
	{
		// try to find in using list
		foreach (scope* sp, usingList_.begin(), usingList_.end())
		{
			variable* q = sp->find(name);
			if (q != NULL)
			{
				p = q;
				break;
			}
		}
	}

	if (p != NULL && p->env() == this)
	{
		reg_table_.insert(std::make_pair(name, p));
	}
	else if (p != NULL && p->env() != this)
	{
		refList_.push_back((scope*)p->env());
	}

	return p;
}

template<typename _SessionType>
_SessionType* ref_session(scope* sp, scope::session_map& smap)
{
	const _Str sname = typeid(_SessionType).name();
	scope::session_map::iterator iter = smap.find(sname);
	_SessionType* psession = NULL;
	if (iter == smap.end())
	{
		psession = new _SessionType(sp);
		smap[sname] = psession;
		if (psession->name() != typeid(_SessionType).name())
			fire("session name is not equal");
		logstring("scope(%s) create new session[%s]", sp->name().c_str(), psession->name().c_str());
		//smap.insert(std::make_pair(sname, psession));
	}
	else 
	{
		psession = as<_SessionType>(iter->second);
	}
	return psession;
}

variable* scope::entry_variable(const _Str& vname, const type* vtype, int ass_type, int32 vscope)
{ 
    logstring("scope(%s): entry_variable, name %s", name_.c_str(), vname.c_str());
    //variable v(vname, vtype, this);
    variable* pv = new variable(vname, vtype, this);
	ref_session<data_session>(this, sessions_)->insert(pv);
	
    return pv;
}
    
variable* scope::entry_function(int ass_type, const _Str& fname, function_type* ftype)
{
    logstring("scope(%s): entry_function, name %s", name_.c_str(), fname.c_str());

	variable* pv = new variable(fname, ftype, this);
	ref_session<data_session>(this, sessions_)->insert(pv);
	return pv;
}

variable* scope::entry_value(const _Str& content, const type* canTypes[], int _C)
{
    logstring("scope(%s): entry_value, content %s", name_.c_str(), content.c_str());
    std::auto_ptr<variable> var;
    value* v = new value();
	_Str name = stringX::format("##noname_var%d$$", noname_idx_ ++);
    if (content.find('.') != _Str::npos)
    {
        v->initv = (byte*)new float(stringX::tovalue<float>(content));
        v->size = sizeof(float);
        var.reset(new variable(name, typesystem::instance().float_type(), this, v));
    }
    else
    {
        v->initv = (byte*)new int(stringX::tovalue<int>(content));
        v->size = sizeof(int);
        var.reset(new variable(name, typesystem::instance().int_type(), this, v));
    }

	ref_session<rodata_session>(this, sessions_)->insert(var.get());

	return var.release();
}

tuple* scope::entry_tuple(const operation* oper, const object* src1, const object* src2, const object* dst)
{
	four_tuple* p = new four_tuple();
	p->dst = (variable*)compile::as<variable>(dst);
	p->oper = oper->oper;
	p->src1 = (variable*)compile::as<variable>(src1);
	p->src2 = (variable*)compile::as<variable>(src2);
	ref_session<text_session>(this, sessions_)->tuples().push_back(p);
    return p;
}
