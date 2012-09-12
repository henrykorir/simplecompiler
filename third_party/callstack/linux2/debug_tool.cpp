#include "debug_tool.hpp"

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <execinfo.h>
#include <signal.h>
#include <sstream>

using namespace kog;

bool get_addr(const char* str, char* buf2, int buf_len)
{
    int len_end = strlen(str);
    while (len_end > 0 && str[len_end - 1] != ']') -- len_end; 
    if (len_end <= 0) 
    {
        return false;
    }
    int len_start = len_end - 1;
    while(len_start >= 0 && str[len_start] != '[') -- len_start;
    if (len_start < 0)
    {
        return false;
    }

    for (int i = len_start + 1; i < len_end - 1; ++ i)
    {
        if (!(str[i] == 'x' || str[i] == 'X' || isxdigit(str[i])))
            return false;
    }
    memset(buf2, 0, buf_len);
    memcpy(buf2, str + len_start + 1, len_end - 1 - len_start - 1);
    return true;
}

void stacktrace::frames(std::vector<std::string>& trace)
{
	void *bt[1024]; 
	char **strings; 
	size_t sz = backtrace(bt, 1024); 
	strings = backtrace_symbols(bt, sz); 
    char apppath[256];
    char buf1[1024];
    char buf2[1024];
    char funcname[128];
    memset(apppath, 0, sizeof(apppath));
    int r = readlink("/proc/self/exe", apppath, sizeof(apppath));
	for(size_t i = 1; i < sz; ++i) // don't show this function
	{
        memset(buf1, 0, sizeof(buf1));
        if (!get_addr(strings[i], buf2, sizeof(buf2)))
        {
            trace.push_back(strings[i]);
            continue;
        }

        sprintf(buf1, "addr2line -f -e %s %s", apppath, buf2);
        FILE* fp = popen(buf1, "r");
        memset(buf1, 0, sizeof(buf1));
        memset(funcname, 0, sizeof(funcname));
        fgets(funcname, sizeof(funcname), fp);
        fgets(buf1, sizeof(buf1), fp);
        int len1 = strlen(buf1);
        if (len1 > 0 && buf1[len1 - 1] == '\n')
        {
            buf1[len1 - 1] = '\0';
        }
        pclose(fp);
        
        sprintf(buf2, "c++filt %s", funcname);
        fp = popen(buf2, "r");
        memset(buf2, 0, sizeof(buf2));
        fgets(buf2, sizeof(buf2), fp);
        int len2 = strlen(buf2);
        if (len2 > 0 && buf2[len2 - 1] == '\n')
        {
            buf2[len2 - 1] = '\0';
        }
        pclose(fp);
        
        std::ostringstream oss;
        oss<<"["<<buf2<<"]"<<buf1;
		trace.push_back(oss.str());
	}
}
