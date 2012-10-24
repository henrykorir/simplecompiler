#include "typeinfoname.h"
#include <stdexcept>

#if (defined _WIN32) || (defined WIN32)
#include <windows.h>
#include <Dbghelp.h>
#else
#include <cstdio>
#include <cstdlib>
#include <cstring>
#endif

namespace kog {

std::string typeinfo_name(const std::string& name)
{
	// use c++filt
	/*std::deque<char> buf;
	const char* ptr = name.c_str();
	while (*ptr)
	{
		if (*ptr == 'N')
		{
			// namespace
			int count = 0;
			while (*ptr >= '0' && *ptr <= '9')
			{
				count = count * 10 + (*ptr - '0');
				++ ptr;
			}
			for (int i = 0; i < count && *ptr; ++ i, ++ ptr)
				buf.push_back(*ptr);
			buf.push_back(':');
			buf.push_back(':');
		}
		else 
	}*/
	char buf[1024];
#if (defined _WIN32) || (defined WIN32)
	UnDecorateSymbolName(name.c_str(), buf, sizeof(buf), 0);
#else
	FILE* fp = popen(("c++filt _Z" + name).c_str(), "r");
	if (fp == NULL) throw std::runtime_error(("can't execute c++filt " + name).c_str());
	
	memset(buf, 0, sizeof(buf));
	fread(buf, sizeof(char), sizeof(buf), fp);
	fclose(fp);
	// remove '\n' @last
	char* p = buf;
	for (int i = 0; i < sizeof(buf) && *p && !(*p == '\r' || *p == '\n'); ++i, ++ p);
	for (int i = p - buf; i < sizeof(buf) && *p; ++i, ++ p) *p = '\0';
#endif
	
	return buf;
}

std::string typeinfo_name(const std::type_info& type)
{
	return typeinfo_name(type.name());
}
}
