// trace_error.h : save callstack in exception
//

#ifndef _TRACE_ERROR_H_KOG_
#define _TRACE_ERROR_H_KOG_

#include <sstream>
#include <string>
#include <stdexcept>
#include <vector>
#include <iterator>

#include <macros.h>
#include "../third_party/callstack/stacktrace.h"

NAMESPACE_BEGIN(kog);

class trace_error : public std::runtime_error
{
public:
	typedef std::string tstring;
public:
	trace_error()
		: std::runtime_error("")
	{
		make_trace();
	}

	trace_error(const tstring& err)
		: std::runtime_error(err)
	{
		make_trace();
	}

	trace_error(const trace_error& other)
		: std::runtime_error(other)
		, trace_(other.trace_)
	{
	}

	virtual ~trace_error() throw()
	{
	}
public:
	tstring trace_message() const
	{
		std::string msg;
		std::ostringstream oss;
		std::copy(trace_.begin(), trace_.end(), std::ostream_iterator<std::string>(oss, "\n"));
		return msg;
	}
private:
	void make_trace()
	{
		kog::callstack::trace ct;
		size_t n = std::distance(ct.begin(), ct.end());
		if (n < 2) return;
		trace_.resize(n - 2);
		kog::callstack::trace::const_iterator begin = ct.begin();
		++ begin; ++ begin;
		std::copy(begin, ct.end(), trace_.begin());
	}
private:
	std::vector<std::string> trace_;

};

/*class argument_exception_base : public scerror
{
public:
	argument_exception_base()
	{
	}

	argument_exception(const argument_exception_base& other)
		: scerror(other)
	{}
	
	argument_exception(const tstring& err, const std::exception& ex)
		: innerException_()
private:
	std::exception* innerException_;
};

public argue*/

NAMESPACE_END(kog);

#endif
