// lalr1machine.h: lalr1 parse machine, read symbol and do actions
//

#ifndef _LALR1_MACHINE_H_
#define _LALR1_MACHINE_H_

#include <lrmachine.h>
#include "ifunction.h"
#include "word.h"
#include "variable.h"

NAMESPACE_BEGIN(compile)


class lalr1machine : public compile::lrmachine
{
public:
    struct lalr1meta : public lrmeta
    {
        lalr1meta(int32 meta = -1)
            : lrmeta(meta)
            , content(NULL)
            , ctype(NULL)
        {}

        lalr1meta(const lalr1meta& other)
            : lrmeta(other)
            , content(other.content)
            , ctype(other.ctype)
        {}

        const object* content;
        const type* ctype;
    };
public:
	lalr1machine();
	lalr1machine(const tinygrammar& g, const kog::smart_vector<ifunction*>& funclist); // all funcion will delete when ~lalr1machine
	virtual ~lalr1machine();
public:
    /* overwrite */ virtual machine_meta* new_meta(int meta);
    /* overwrite */ virtual machine_meta* new_meta(const machine_meta* meta);
protected:
	/* overwrite */ virtual machine_meta* _reduce(int32 pid, const kog::smart_vector<machine_meta*>& rights, machine_meta* result);
private:
	kog::smart_vector<ifunction*> funcList_;
	std::allocator<lalr1meta> alloc_;
};

NAMESPACE_END(compile)

#endif
