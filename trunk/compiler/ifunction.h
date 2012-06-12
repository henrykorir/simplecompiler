// ifunction.h : production function
//

#ifndef _IFUNCTION_H_SC_
#define _IFUNCTION_H_SC_

#include <macros.h>
#include <basic_types.h>
#include <automachine.h>

NAMESPACE_BEGIN(compile)

class ifunction
{
public:
    typedef automachine::machine_meta machine_meta;
    //typedef lalr1machine::lalr1meta compileitem;
public:
    virtual machine_meta* operator()(machine_meta*const* metas, int C, machine_meta* result) = 0;
};

NAMESPACE_END(compile)
#endif
