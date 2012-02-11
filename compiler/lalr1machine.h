// lalr1machine.h: lalr1 parse machine, read symbol and do actions
//

#ifndef _LALR1_MACHINE_H_
#define _LALR1_MACHINE_H_

NAMESPACE_BEGIN(compiler)


class lalr1machine
{
public:
	lalr1machine();
	virtual ~lalr1machine();
public:
	bool eta(const symbol* sym);
private:
	int32 cstate_;
};

NAMESPACE_END(compiler)
#endif
