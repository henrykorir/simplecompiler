#include "debug_tool.hpp"

#include <signal.h>
#include<cstdlib>
#include <execinfo.h>
#include <iostream>

using namespace kog;

void stacktrace::frames(std::vector<std::string>& trace)
{
	void *bt[1024]; 
	char **strings; 
	size_t sz = backtrace(bt, 1024); 
	strings = backtrace_symbols(bt, sz); 
	for(size_t i = 0; i < sz; ++i)
	{
		trace.push_back(std::string(strings[i]));
		std::cout<<strings[i]<<std::endl;
	}
}
