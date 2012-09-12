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
	//	fire("no production function[%d]!", pid);
	}
    else if(funcList_[pid] != NULL)
    {
		logstring("reduce product(%d)", pid);
        (*funcList_[pid])(rights.get(), rights.size(), result);
    }
	return result;
}

machine_meta* lalr1machine::new_meta(int32 meta)
{
    machine_meta* pmeta = new lalr1meta;
    pmeta->sid = meta;
    return pmeta;
}

machine_meta* lalr1machine::new_meta(const machine_meta* meta)
{
    kog::assert::is_true(meta != NULL);
    //kog::assert::is_true(dynamic_cast<const lalr1meta*>(meta) != NULL);
    return new lalr1meta(*((const lalr1meta*)meta));
}
