#include "lrmachine.h"
#include <scerror.h>

using namespace compile;

void lrmachine::init()
{
	automachine::init();
//	EOF(-1): end of the input file
	pstack_.push(stackitem(sstate_, -1));
}

bool lrmachine::eta(int32 meta)
{
	if(!automachine::eta(meta)) return false;
	while(cstate_ < 0)
	{
		int32 newMeta = reduce(- (cstate_ + 1));
		if(!eta(newMeta)) return false;
		if(meta == -1 && cstate_ == accept_state &&
			pstack_.size() == 1) return true; // accept
		if(!automachine::eta(meta)) return false;
	}
	if(cstate_ == accept_state) // accept, we use state 0 as the accept status
	{
	}
	else pstack_.push(stackitem(cstate_, meta));
	return true;
}

int32 lrmachine::reduce(int32 pid)
{
	const production& p = pg_->productions().at(pid);
	const symholder& sholder = pg_->symbols();
	if(pstack_.size() < p.right_size() + 1) fire("invalidate analysis stack!");
	for(int32 i = p.right_size(); i; -- i)
	{
		if(p.right()[i-1] != pstack_.top().meta)
			fire("reduce error production[%d] need %s but gived %s", pid, 
					sholder[p.right()[i-1]].name, sholder[pstack_.top().meta].name);
		pstack_.pop();
	}
	cstate_ = pstack_.top().state;
	return p.left();
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