// prebuild.h: pre-build c file
//

#ifndef _PREBUILD_H_SC_
#define _PREBUILD_H_SC_

#include <compiler.h>
#include <compilersyntax.h>
#include <lalr1machine.h>
#include <symbolmachine.h>
#include <buckethash.h>

NAMESPACE_BEGIN(compile);
NAMESPACE_BEGIN(cplc);

class prebuildcfile
{
public:
	prebuildcfile();
	virtual ~prebuildcfile();
public:
	tostream& operator()(tistream& is, tostream& os);
private:
	tistream* is_;
	tostream* os_;
};

NAMESPACE_END(cplc);
NAMESPACE_END(compile);

#endif
