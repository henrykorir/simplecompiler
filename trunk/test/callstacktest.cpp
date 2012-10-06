#include <iostream>
#include "../third_party/callstack/stacktrace.h"

class A
{
public:
	A()
	{
		/* track call stack */
		/* comment test line1
			comment test line2 */
		kog::callstack::trace trace;
		for (kog::callstack::trace::const_iterator iter = trace.begin(); iter != trace.end(); ++ iter)
		{
			std::cout<<*iter<<std::endl;
		}
	}
};

int main(int argc, const char* argv[])
{
	A a; // show callstack
	// line comments \
		newline
	return 0;
}