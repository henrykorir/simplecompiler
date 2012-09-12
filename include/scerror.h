// scerror.h: define exception 
//

#ifndef _SCERROR_H_SC_
#define _SCERROR_H_SC_

#include <macros.h>
#include <stdexcept>

#include <basic_types.h>
#include <stringX.h>
#include <functionalX.h>
#include <shared_ptr.h>
#include "../third_party/callstack/stacktrace.h"

NAMESPACE_BEGIN(sc)

class scerror : public std::runtime_error
{
public:
	scerror()
		: std::runtime_error("")
	{
		make_trace();
	}

	scerror(const tstring& err)
		: std::runtime_error(err)
	{
		make_trace();
	}

	~scerror() throw()
	{
	}
public:
	tstring trace_message() const
	{
		tstring msg;
		foreach (const std::string& frame, trace_->begin(), trace_->end())
		//for (kog::callstack::trace::const_iterator iter = ; iter != ; ++ iter)
		{
			msg += frame + "\n";
		}
		return msg;
	}
private:
	void make_trace()
	{
		trace_ = new kog::callstack::trace;
	}
private:
	kog::shared_ptr<kog::callstack::trace> trace_;
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
