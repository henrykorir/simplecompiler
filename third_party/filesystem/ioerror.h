// ioerror.h : 
// 

#ifndef _IOERROR_H_KOG_
#define _IOERROR_H_KOG_

#include <string>
#include <macros.h>

#include "trace_error.h"

NAMESPACE_BEGIN(kog);

class ioerror : public kog::trace_error
{
public:
	ioerror()
	{}

	ioerror(const tstring& err)
		: kog::trace_error(err)
	{
	}

	ioerror(const ioerror& other)
		: kog::trace_error(other)
	{
	}
};

NAMESPACE_END(kog);

#endif