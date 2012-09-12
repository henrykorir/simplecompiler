#include "lrmachine.h"
#include <scerror.h>
#include <logger.h>

using namespace compile;

void lrmachine::init()
{
	automachine::init();
//	EOF(-1): end of the input file
    machine_meta* pmeta = new_meta(-1);
    ((lrmeta*)pmeta)->state = sstate_;
	pstack_.push(pmeta);
}

automachine::machine_meta* lrmachine::new_meta(int meta)
{
	return new lrmeta(meta);
}

automachine::machine_meta* lrmachine::new_meta(const automachine::machine_meta* meta)
{
    return new lrmeta(*((const lrmeta*)meta));
}

bool lrmachine::eta(machine_meta* meta)
{
	if(!automachine::eta(meta)) return false;
	while(cstate_ < 0)
	{
		machine_meta* newMeta = reduce(- (cstate_ + 1));
		if(!eta(newMeta)) return false;
		if(meta->sid == -1 && cstate_ == accept_state &&
			pstack_.size() == 1) return true; // accept
		if(!automachine::eta(meta)) return false;
	}
	if(cstate_ == accept_state) // accept, we use state 0 as the accept status
	{
        logstring("accept!");
	}
	else 
    {
        lrmeta* pmeta = (lrmeta*)new_meta(meta);
        pmeta->state = cstate_;
        pstack_.push(pmeta);
    }
	return true;
}

lrmachine::machine_meta* lrmachine::reduce(int32 pid)
{
	const production& p = pg_->productions().at(pid);
	
    const symholder& sholder = pg_->symbols();
	if((int32)pstack_.size() < p.right_size() + 1) fire("invalidate analysis stack!");
    kog::smart_vector<machine_meta*> rights(p.right_size());
	for(int32 i = p.right_size(); i; -- i)
	{
		if(p.right()[i-1] != pstack_.top()->sid)
			fire("reduce error production[%d] need %s but gived %s", pid, 
					sholder[p.right()[i-1]].name, sholder[pstack_.top()->sid].name);
        rights[i-1] = pstack_.top();
		pstack_.pop();
	}
	cstate_ = ((lrmeta*)pstack_.top())->state;
    
    machine_meta* pmeta = new_meta(p.left());
    pmeta->sid = p.left();
	machine_meta* presult = _reduce(pid, rights, pmeta);

    for (size_t i = 0; i < rights.size(); ++ i)
    {
        delete rights[i];
        rights[i] = NULL;
    }
    rights.reset(0);
    return presult;
}

lrmachine::machine_meta* lrmachine::_reduce(int32 pid, const kog::smart_vector<machine_meta*>& rights, machine_meta* result)
{
	return result;
}

void lrmachine::swap(lrmachine& other) throw()
{
	((automachine*)this)->swap(other);
	//pstack_.swap(other.pstack_);
	std::swap(pstack_, other.pstack_);
	funcs_.swap(other.funcs_);
	morelist_.swap(other.morelist_);
	std::swap(pg_, other.pg_);
}
