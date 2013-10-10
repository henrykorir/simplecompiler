#include "prebuild.h"
#include <ilinestream.h>
#include <regex/xregex.h>
#include <logger.h>

using namespace compile;
using namespace compile::cplc;

prebuildcfile::prebuildcfile()
{
}

prebuildcfile::~prebuildcfile()
{
}


tostream& prebuildcfile::operator()(tistream& is, tostream& os)
{
	
	return os;
}

tstring prebuildcfile::get_string_content(const tstring& s) const
{
	tstring buffer;
	const tchar* p = s.c_str();
	while (*p)
	{
		if (*p == '\\' && *(p+1) != '\0')
		{
			switch (*(++ p))
			{
			case 'r': buffer.push_back('\r'); break;
			case 'n': buffer.push_back('\n'); break;
			case 'f': buffer.push_back('\f'); break;
			case 't': buffer.push_back('\t'); break;
			case 'v': buffer.push_back('\v'); break;
			case 'b': buffer.push_back('\b'); break;
			case '"': buffer.push_back('"'); break;
			case '\\':buffer.push_back('\\'); break;
			default:
				buffer.push_back(*p); break;
				logwarning("invalidate \\%c", *p);
			}
		}
		else if (*p == '\\')
		{
			logwarning("invalidate \\");
			buffer.push_back('\\');
		}
		else
		{
			buffer.push_back(*p);
		}
		++ p;
	}
	return buffer;
}

