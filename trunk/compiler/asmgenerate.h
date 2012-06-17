// asmgenerate.h: generate asm code
//

#ifndef _ASM_GENERATE_H_SC_
#define _ASM_GENERATE_H_SC_

#include "scope.h"

NAMESPACE_BEGIN(compile);
NAMESPACE_BEGIN(runtime);

class asmgenerate
{
public:
    asmgenerate(std::ostream& os);
public:
    void print(scope* s);
private:
    std::ostream* os_;
};

NAMESPACE_END(runtime);
NAMESPACE_END(cpmoile); 

#endif
