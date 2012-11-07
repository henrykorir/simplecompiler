// scerror.h: define exception 
//

#ifndef _SCERROR_H_SC_
#define _SCERROR_H_SC_

#include <macros.h>
#include <stdexcept>
#include <stringX.h>

#include <basic_types.h>
#include "trace_error.h"

NAMESPACE_BEGIN(sc)

class scerror : public kog::trace_error
{
public:
	scerror()
	{
	}

	scerror(const tstring& err)
		: kog::trace_error(err)
	{
	}

	scerror(const scerror& other)
		: kog::trace_error(other)
	{
	}
};

#ifdef VISUAL_STDIO
#define fire(fmt, ...) \
	throw sc::scerror(stringX::format("%s:%d ", __FILE__, __LINE__) + stringX::format(fmt, __VA_ARGS__))
#else
#define fire(fmt, arg...) \
	throw sc::scerror(stringX::format(fmt, ##arg))
#endif
NAMESPACE_END(sc)

#endif
