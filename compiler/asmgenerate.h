// asmgenerate.h: generate asm code
//

#ifndef _ASM_GENERATE_H_SC_
#define _ASM_GENERATE_H_SC_

#include "module.h"

NAMESPACE_BEGIN(compile);
NAMESPACE_BEGIN(runtime);

class asmgenerate
{
public:
    asmgenerate(std::ostream& os);
public:
    void print(scope* s);
	void print(module* m);
protected:
	void print_code(const _Str& name, text_session* ss);
	void print_printasm();
private:
    std::ostream* os_;
};

NAMESPACE_END(runtime);
NAMESPACE_END(cpmoile); 

#endif
