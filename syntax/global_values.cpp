#include "global_values.h"
#include <fstream>
#include <stringX.h>
#include <scerror.h>

using namespace kog;

#if LOAD_GLOBAL_VALUES_FROMFILE

#include <stack>
#include <xml/Markup.h>

//struct xmlnode
//{
//	std::string tagname;
//	std::string name;
//	std::string type;
//	std::string value;
//	std::string more;
//};

//typedef tree<xmlnode> xmltree;

typedef xml::CMarkup xmlfile;

struct value_t
{
	std::string name; // name of value
	std::string type;
	std::string value;
	std::string more;
};

std::string global_values::load_file_ = "";

/*std::string& getprop(xmlNodePtr node, const std::string& name, std::string& content)
{
	if(xmlChar* v = xmlGetProp(node, (const xmlChar*)name.c_str()))
	{
		content = (const char*)(v);
		xmlFree(v);
	}
	else content.clear();
	return content;
}*/

void readxml(xmlfile& doc, std::deque<value_t>& values)
{
	if(!doc.IntoElem()) return;

	const static std::string nametag = "value";
	const static std::string nameprop = "name";
	const static std::string typeprop = "type";
	const static std::string more = "more";

	while(doc.FindElem())
	{
		std::string ctag = doc.GetTagName();
		if (nametag.empty()) continue;
		else if(nametag == stringX::tolower(ctag))
		{
			value_t v;
			v.name = doc.GetAttrib(nameprop);
			v.type = doc.GetAttrib(typeprop);
			v.more = doc.GetAttrib(more);
			v.value = doc.GetData();
			values.push_back(v);
		}
		else 
		{
			readxml(doc, values);
		}
	}
	doc.OutOfElem();
}

void readfile(const std::string& file, std::deque<value_t>& values)
{
	xmlfile xmldoc;
	if(!xmldoc.Load(file)) fire("can't auto load file: " + file);
	if(!xmldoc.FindElem() || stringX::tolower(xmldoc.GetTagName()) != "global_values") 
		fire("xmlfile, not root!");
	readxml(xmldoc, values);
}

template<typename _T, typename _Char>
_T convert(const std::basic_string<_Char>& v)
{
	return stringX::tovalue<_T>(v);
}

void global_values::load_from_file()
{
	if(load_file_.empty()) return;
	
	std::deque<value_t> values;
	readfile(load_file_, values);

	for(std::deque<value_t>::const_iterator iter = values.begin(); iter != values.end(); ++ iter)
	{
		if(iter->name.empty() || iter->type.empty()) continue;
		if(iter->type == "int"){
			insert(iter->name, convert<int>(iter->value));
		}else if(iter->type == "double"){
			insert(iter->name, convert<double>(iter->value));
		}else if(iter->type == "string"){
			insert(iter->name, iter->value);
		}
	}
}

#endif
