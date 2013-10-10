#include "scope.h"
#include <cstdarg>
#include <memory>
#include <typeinfo>
#include <stringX.h>
#include <functionalX.h>
#include <logger.h>
#include "type_helper.h"

using namespace compile;
using namespace compile::runtime;

scope* compile::runtime::global_scope = /*new slicescope*/NULL;

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
	if (name.size() > 4 && name.find("$ro$") == 0)
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
	if (find_here(vname) != NULL)
	{
		logerror("find existing variable!");
		return NULL;
	}

    variable* pv = new variable(vname, vtype, this);
	ref_session<data_session>(this, sessions_)->insert(pv);
	
    return pv;
}

static int g_tmpid = 0;
tstring scope::get_temp_name()
{
	return name_ + stringX::format("%d", g_tmpid++);
}

variable* scope::entry_function(int ass_type, const _Str& fname, function_type* ftype)
{
    logstring("scope(%s): entry_function, name %s", name_.c_str(), fname.c_str());

	variable* pv = new variable(fname, ftype, this);
	ref_session<data_session>(this, sessions_)->insert(pv);
	return pv;
}

variable* scope::entry_value(const _Str& content, const type* var_type)
{
    logstring("scope(%s): entry_value, content %s, type %s", name_.c_str(), content.c_str(), get_typeinfo(var_type));
	if (var_type == NULL) fire("var type == NULL");
	rodata_session* rods = ref_session<rodata_session>(this, sessions_);
	const tstring tname = stringX::format("$ro$[%s]%s", get_typeinfo(var_type), content.c_str());
	variable* var = find_here(tname);
	if (var == NULL)
	{ // not found existing value, create new one
		value* v = new value;
		v->size = (uint32)var_type->tsize;
		if (var_type == int_type) v->initv = (byte*)new int(atoi(content.c_str()));
		else if (var_type == float_type) v->initv = (byte*)new float((float)atof(content.c_str()));
		else if (var_type == double_type) v->initv = (byte*)new double(atof(content.c_str()));
		else if (var_type == char_type) v->initv = (byte*)new char(content[0]);
		//else if (var_type == tuple_type) v->initv = new 
		var = new variable(tname, var_type, this, v);

		rods->insert(var);
	}

	return var;
}

variable* scope::entry_value(const _Str& content, const type* canTypes[], int _C)
{
	const type* var_type = NULL;
    if (content.find('.') != _Str::npos)
    {
        var_type = typesystem::instance().get_type("float");
    }
    else
    {
        var_type = typesystem::instance().get_type("int");
    }

	return entry_value(content, var_type);
}

tuple* scope::entry_tuple(const operation* oper, const object* src1, const object* src2, const object* dst)
{
	four_tuple* p = new four_tuple();
	p->dst = (variable*)compile::as<variable>(dst);
	p->oper = oper->oper;
	p->src1 = (variable*)compile::as<variable>(src1);
	p->src2 = (variable*)compile::as<variable>(src2);
	ref_session<text_session>(this, sessions_)->tuples().push_back(p);
	logstring("enter tuple %s = %s(%s, %s)", obj2str(dst).c_str(), oper->to_string().c_str(), obj2str(src1).c_str(), obj2str(src2).c_str());
    return p;
}
	
const session* scope::get_session(const _Str& session_name) const
{
	session_map::const_iterator iter = sessions_.find(session_name);
	if (iter == sessions_.end()) return NULL;
	return iter->second;
}

int32 scope::access_type(const _Str& str) const
{
	return 0;
}
