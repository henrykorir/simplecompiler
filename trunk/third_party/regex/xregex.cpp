#include "xregex.h"
#include <boost/regex.hpp>

namespace stringX{

xregex::xregex(void)
{
}

xregex::~xregex(void)
{
}

bool is_match(const boost::regex& regex, const xregex::tstring& content)
{
	boost::smatch what;
	return boost::regex_match(content, what, regex);
}


bool xregex::is_interge(const tstring& str)
{
	static boost::regex isInterge("[+-]?\\d+[LDO]");
	return stringX::is_match(isInterge, str);
}

bool xregex::is_id(const tstring& str)
{
	static boost::regex isID("[a-zA-Z_][a-zA-Z_0-9]*");
	return stringX::is_match(isID, str);
}

bool xregex::is_match(const tstring& regex_str, const tstring& content)
{
	boost::regex _regex(regex_str);
	return stringX::is_match(_regex, content);
}

xregex::tstring xregex::replace_all(const tstring& regex_str, 
		const tstring& replace_str, 
		const tstring& org_str)
{
	boost::regex _regex(regex_str);
	return boost::regex_replace(org_str, _regex, replace_str);
}

} // end of namespace stringX
