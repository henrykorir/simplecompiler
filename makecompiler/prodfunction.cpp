#include "prodfunction.h"
#include <stringX.h>
#include <regex/xregex.h>
#include <logger.h>

using namespace compile;
using namespace compile::syntax;

compiler_action_parser::compiler_action_parser(funcinfo_t& otput)
: otput_(&otput)
{
}

bool compiler_action_parser::operator()(const tstring& line)
{
	const tstring& tmp = line;
	std::vector<tstring>& content = otput_->content;
	size_t npos = tmp.find(':');
	if (npos != tstring::npos)
	{
		tstring L0 = stringX::trim(tmp.substr(0, npos));
		tstring L1 = stringX::trim(tmp.substr(npos + 1));

		if (L0 == "$$") 
		{
			content.push_back(L1 + "& _ddst = *dynamic_cast<" + L1 +"*>((lalr1meta*)dst->content)");
		}
		else if (stringX::xregex::is_match("\\$[0-9]+", L0)) 
		{
			tstring n = L0.substr(1);
			content.push_back(L1 + "& _vp" + n + " = *dynamic_cast<" + L1 +"*>((lalr1meta*)metas[" + n + "]->content)");
		}
		else if (stringX::xregex::is_match("#[0-9]+", L0))
		{
			tstring n = L0.substr(1);
			content.push_back("object &obj" + n + " = " + L1);
		}
		else 
		{
			logerror("invalidate " + L0);
			return false;
		}
	}
	else
	{
		// replace all $$ with _ddst
		// 	and $0 with _vp0 ...
		content.push_back(
			stringX::replaceall(
				stringX::replaceall(
					stringX::replaceall(tmp, std::string("$$"), std::string("_ddst")), 
				std::string("$"), std::string("_vp")),
			std::string("#"), std::string("obj")));
	}
	return true;
}