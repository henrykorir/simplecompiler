#include "prodfunction.h"
#include <stringX.h>
#include <regex/xregex.h>
#include <logger.h>
#include <deque>

using namespace compile;
using namespace compile::syntax;

compiler_action_parser::compiler_action_parser(funcinfo_t& otput)
: otput_(&otput)
, get_left_start_(false)
{
}

bool compiler_action_parser::newline(const tstring& line)
{
	if (line == "{" && !get_left_start_)
	{
		get_left_start_ = true;
		return true;
	}
	else if (line == "{")
	{
		fire("invalidate production");
	}
	else if (line == "}" && get_left_start_)
	{ // end of the production function
		get_left_start_ = false;
		return false;
	}
	else if (line == "}" && !get_left_start_)
	{
		fire("invalidate production");
	}

	tstring tmpline;
	if (line[0] == '\t') tmpline = line.substr(1);
	else if (line.size() >= 4 && line.substr(0, 4) == "    ") tmpline = line.substr(4);
	else tmpline = line;
	return (*this)(tmpline);
}

bool compiler_action_parser::operator()(const tstring& line)
{
	const tstring& tmp = line;
	std::vector<tstring>& content = otput_->content;
	// replace all $$ with _ddst
	// 	and $0 with _vp0 ...
	//std::deque<tchar> buffer;
	

	tstring var, type;
	if (_is_declear(line, var, type))
	{
		tstring buffer;
		if (var[0] == '$') 
		{
			if (var == "$$") fire("can't set result type");
			else var = "_m" + var.substr(1);

			buffer = stringX::format("%s* %s = (%s*)(((lalr1meta*)metas[%s])->content);", 
				type.c_str(), var.c_str(), type.c_str(), var.substr(2).c_str());
		}
		else 
		{
			var = "_t" + var.substr(1);

			if (type.find_first_of("#$") != std::string::npos)
			{
				std::vector<std::string> tmp;
				if (_is_exp(type, tmp))
				{
					type = tmp[0];
				}
				else
				{
					fire("invalidate type");
				}
			}

			const tstring variable_tag = "variable";
			const tstring complex_var_tag = "complex_variable";
			const tstring type_var_tag = "type_variable";
			const tstring pointer_var_tag = "pointer_variable";
			const tstring arraylist_var_tag = "arraylist";
			if (type == complex_var_tag || type == type_var_tag || type == pointer_var_tag || type == arraylist_var_tag)
			{
				buffer = stringX::format("%s* %s = new %s(get_temporary_name(), get_curenv());", 
					type.c_str(), var.c_str(), type.c_str());
			}
			else
			{
				buffer = stringX::format("%s* %s = new %s(get_temporary_name(), %s, get_curenv());", 
					variable_tag.c_str(), var.c_str(), variable_tag.c_str(), type.c_str());
			}
		}
		content.push_back(buffer);
	}
	else if (!_is_exp(line, content))
	{
		fire("invalidate expression");
	}

	
//	content.push_back(
//		stringX::replaceall(
//			stringX::replaceall(
//				stringX::replaceall(tmp, std::string("$$"), std::string("_ddst")), 
//			std::string("$"), std::string("_vp")),
//		std::string("#"), std::string("obj")));
	return true;
}

bool compiler_action_parser::_is_declear(const tstring& line, std::string& var, std::string& type) const
{
	size_t pos = line.find(':');
	if (pos == tstring::npos) return false;

	const char* p = line.c_str();
	const char* e = p + pos;
	while (p < e && ::isspace(*p)) ++ p;
	// p stop at first non-space
	if (*p == ':') return false;
	else if (*p == '$' || *p == '#')
	{
		if (p + 1 >= e) return false;
		else if (p[0] == '$' && p[1] == '$')
		{
			var = "$$";
			++ p;
		}
		else
		{
			var.push_back(*p);
			++ p;
			while (p < e && *p <= '9' && *p >= '0')
			{
				var.push_back(*p);
				++ p;
			}
			if (var.size() <= 1) return false;
		}
		// check white space
		while (p < e && ::isspace(*p)) ++ p;
		if (p != e) return false; 
	}
	else return false;

	p = line.c_str() + pos + 1;
	e = line.c_str() + line.size();
	while (p < e && ::isspace(*p)) ++ p;
	while (p < e && *p != '\n') type.push_back(*(p ++));
	stringX::trim(type);
	//while (p < e && ::isspace(*p)) ++ p;

	if (type.empty() || p != e) return false;

	return true;
}

bool compiler_action_parser::_is_exp(const tstring& line, std::vector<tstring>& content)
{
	tstring buffer;
	const tchar* p = line.c_str();
	int middle_brace = 0;
	while (*p != '\0')
	{
		switch (*p)
		{
		case '$':
			{
				char cn = *(p + 1);
				if (cn == '\0') return false;
				else if (cn == '$') 
				{
					buffer.append("resvar");
					
					p += 2;
				}
				else if (cn <= '9' && cn >= '0')
				{
					buffer.append("_m");
					++ p;
					// a param
					while (*p <= '9' && *p >= '0')
					{
						buffer.push_back(*p);
						++ p;
					}
				}
				else return false;
			}
			break;
		case '#':
			{
				char cn = *(p + 1);
				if (cn == '\0') return false;
				else if (cn <= '9' && cn >= '0')
				{
					buffer.append("_t");
					++ p;
					while (*p <= '9' && *p >= '0')
					{
						buffer.push_back(*p);
						++ p;
					}
				}
				else return false;
			}
			break;
		case '.':
			buffer.append("->");
			++ p;
			break;
		case '[':
			if (p[1] == '[')
			{
				buffer.append("tolist(");
				++ middle_brace;
				++ p;
			}
			else
			{
				buffer.push_back(*p);
			}
			++ p;
			break;
		case ']':
			if (middle_brace != 0 && p[1] == ']')
			{
				const tchar* q = p - 1;
				while (q != line.c_str() - 1 && ::isspace(*q)) --q;
				if (q == line.c_str() - 1 || q == line.c_str())
				{
					buffer.push_back(*p);
				}
				else if (*q == '[' && q[-1] == '[')
				{
					buffer.append("NULL)"); // empty list
					-- middle_brace;
					++ p;
				}
				else 
				{
					buffer.append(", NULL)");
					-- middle_brace;
					++ p;
				}
			}
			else buffer.push_back(*p);
			++ p;
			break;
		case 't':
			// check type
			if (p != line.c_str() && p[-1] == '.' &&
				p[1] == 'y' && p[2] == 'p' && p[3] == 'e' && !::isalnum(p[4]))
			{
				buffer.append("vtype()");
				p += 4;
			}
			else
			{
				buffer.push_back(*p);
				++ p;
			}
			break;
		case 'm':
			if (p != line.c_str() && p[-1] == '.' &&
				p[1] == 'o' && p[2] == 'r' && p[3] == 'e')
			{
				buffer.append("more()");
				p += 4;
			}
			else
			{
				buffer.push_back(*p);
				++ p;
			}
			break;
		//case 'f':
		//	if (p != line.c_str() && ::isspace(p[-1]) && 
		//		strncmp(p, "foreach", 7) == 0 && (::isspace(p[7]) || p[7] == '('))
		//	{
		//		
		//	} 
		case 'i':
			if (p != line.c_str() && ::isspace(p[-1]) && p[1] == 'n' &&
				::isspace(p[2]) && line.find("foreach") != tstring::npos)
			{ // foreach command
				const tchar* q = p + 3;
				// read util ')'
				while (*q != '\0' && *q != ')') ++ q;
				if (*q == '\0')
				{ // not foreach command
					buffer.push_back(*p);
					++ p;
				}
				else
				{
					tstring name = stringX::trim(std::string(p + 3, q));
					std::vector<tstring> vbuf;
					_is_exp(name, vbuf);
					buffer.append(stringX::format(", %s->begin(), %s->end()", vbuf.at(0).c_str(), vbuf.at(0).c_str()));
					p = q;
				}
			}
			else
			{
				buffer.push_back(*p);
				++ p;
			}
			break;
		default:
			buffer.push_back(*p);
			++ p;
			break;
		}
	}
	content.push_back(buffer);

	return true;
}
