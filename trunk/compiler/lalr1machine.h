// lalr1machine.h: lalr1 parse machine, read symbol and do actions
//

#ifndef _LALR1_MACHINE_H_
#define _LALR1_MACHINE_H_

#include <lrmachine.h>

NAMESPACE_BEGIN(compiler)


class lalr1machine : public compile::lrmachine
{
public:
	lalr1machine();
	virtual ~lalr1machine();
public:
};

NAMESPACE_END(compiler)
#endif
