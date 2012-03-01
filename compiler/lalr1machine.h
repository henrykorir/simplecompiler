// lalr1machine.h: lalr1 parse machine, read symbol and do actions
//

#ifndef _LALR1_MACHINE_H_
#define _LALR1_MACHINE_H_

#include <lrmachine.h>

NAMESPACE_BEGIN(compile)


class lalr1machine : public compile::lrmachine
{
public:
	lalr1machine();
	lalr1machine(const tinygrammar& g) : compile::lrmachine(g){};
	virtual ~lalr1machine();
public:
};

NAMESPACE_END(compile)
#endif
