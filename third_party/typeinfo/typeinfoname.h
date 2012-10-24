// typeinfoname.h: convert typeinfo name
//

#ifndef _TYPEINFO_NAME_KOG_H_
#define _TYPEINFO_NAME_KOG_H_

#include <string>
#include <typeinfo>

namespace kog
{
	std::string typeinfo_name(const std::string& name);
	std::string typeinfo_name(const std::type_info& type);
}

#endif