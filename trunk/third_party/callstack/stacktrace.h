// stacktrace.h: 
//

#ifndef _STACK_TRACE_H_SC_
#define _STACK_TRACE_H_SC_

#if (defined _WIN32) || (defined WIN32)
#include "win32/debug_tool.hpp"
#pragma comment(lib, "Dbghelp.lib")

namespace kog{
namespace callstack
{

	typedef dbsoft::callstack::func_name_list frame;

	class trace
	{
		// non-copyable
		trace(const trace&);
		trace& operator=(const trace&);
	public:
		trace()
		{
			stack_ = dbsoft::callstack::generate();
		}
	public:
		typedef frame::const_iterator const_iterator;
	public:
		const_iterator  begin() const
		{
			return stack_->begin();
		}

		const_iterator  end() const
		{
			return stack_->end();
		}
	private:
		dbsoft::callstack::callstack_ptr stack_;
	};

} // end of callstack
} // end of kog
#else
#include "linux2/debug_tool.hpp"
namespace kog{
namespace callstack{

	typedef std::string frame;
	class trace
	{
			// non-copyable
		trace(const trace&);
		trace& operator=(const trace&);
	public:
		typedef std::vector<frame>::const_iterator const_iterator;
	public:
		trace()
		{
			kog::stacktrace::frames(stack_);
		}
	public:
		const_iterator begin() const { return stack_.begin(); }
		const_iterator end() const { return stack_.end(); }
	private:
		std::vector<frame> stack_;
	};

}
}
#endif

#endif
