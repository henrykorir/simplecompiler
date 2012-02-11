// scerror.h: define exception 
//

#ifndef _SCERROR_H_SC_
#define _SCERROR_H_SC_

#include <macros.h>
#include <stdexcept>

#include <basic_types.h>
#include <stringX.h>

NAMESPACE_BEGIN(sc)

class scerror : public std::runtime_error
{
public:
	scerror()
		: std::runtime_error("")
	{}

	scerror(const tstring& err)
		: std::runtime_error(err)
	{}
};

#ifdef VISUAL_STDIO
#define fire(fmt, ...) \
	throw scerror(stringX::format(fmt, __VA_ARGS__))
#else
#define fire(fmt, arg...) \
	throw scerror(stringX::format(fmt, ##arg))
#endif
NAMESPACE_END(sc)

#endif
