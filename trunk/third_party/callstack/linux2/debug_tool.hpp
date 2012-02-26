// debug_tool.hpp: 
// 
#ifndef _DEBUG_TOOL_LINUX_H_KOG_
#define _DEBUG_TOOL_LINUX_H_KOG_

#include <vector>
#include <string>

namespace kog
{
	struct stacktrace
	{
		static void frames(std::vector<std::string>& stack);
	};
}
#endif
