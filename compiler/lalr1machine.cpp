#include "lalr1machine.h"
#include <logger.h>
#include <scerror.h>
#include <debug.h>

using namespace compile;
//using namespace compile::doc;
//using namespace compile::runtime;
typedef automachine::machine_meta machine_meta;

lalr1machine::lalr1machine()
{
}
	
lalr1machine::lalr1machine(const tinygrammar& g, const kog::smart_vector<ifunction*>& funclist)
    : compile::lrmachine(g)
    , funcList_(funclist)
{
}

lalr1machine::~lalr1machine()
{
    // delete all funcions
    for (kog::smart_vector<ifunction*>::iterator iter = funcList_.begin(); iter != funcList_.end(); ++ iter)
    {
        if (*iter == NULL) continue;
        delete *iter;
        *iter = NULL;
    }
    funcList_.reset(0);
}

machine_meta* lalr1machine::_reduce(int32 pid, const kog::smart_vector<machine_meta*>& rights, machine_meta* result)
{
    if ((int32)funcList_.size() <= pid || pid < 0)
	{
		logstring("not found production reduce function [%d]!", pid);
	//	fire("no production function[%d]!", pid);
	}
    else if(funcList_[pid] != NULL)
    {
		compile::ifunction* func = funcList_[pid];
		prodholder_proxy pholder = tinyg().productions();
		logstring("reduce product(%d): %s", pid, pholder.at(pid).to_string().c_str());
		func->entry();
#ifdef NOTCALL_REDUCE_FUNC
		logstring("don't call ruduce function");
#else
        (*func)(rights.get(), (int)rights.size(), result);
#endif
		func->leave();
    }
	return result;
}

machine_meta* lalr1machine::new_meta(int32 meta)
{
	machine_meta* pmeta = alloc_.allocate(1);
    pmeta->sid = meta;
    return pmeta;
}

machine_meta* lalr1machine::new_meta(const machine_meta* meta)
{
    kog::assert::is_true(meta != NULL);

	lalr1meta* pmeta = alloc_.allocate(1);
	*pmeta = *((const lalr1meta*)meta);
    return pmeta;
}

void lalr1machine::swap(lalr1machine& other) throw()
{
	((lrmachine*)this)->swap(other);
	funcList_.swap(other.funcList_);
	//alloc_.swap(other.alloc_);
	std::swap(alloc_, other.alloc_);
}
