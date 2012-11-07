// folder.h : 
// 

#ifndef _FOLDER_H_KOG_
#define _FOLDER_H_KOG_

#include <string>
#include <macros.h>

#include "ioerror.h"

NAMESPACE_BEGIN(kog);

class folder
{
public:
	typedef std::string tstring;
public:
	folder(const tstring& folder_name);
	virtual ~folder();
public:
	bool isexist() const;
	void mkdir();
private:
	tstring path_;
};

NAMESPACE_END(kog);

#endif