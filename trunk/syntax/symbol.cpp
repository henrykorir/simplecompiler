#include "symbol.h"
#include <algorithm>
#include <scerror.h>

using namespace compile;

void symholder::make_index()
{
	//std::sort(begin(), end(), symbolname_Less());
	nameIndexer_.clear();
	for(const_iterator iter = begin(); iter != end(); ++ iter)
	{
		nameIndexer_[iter->name] = &(*iter);
	}
}

int32 symholder::index(const tstring& name) const
{
	if(nameIndexer_.empty()) fire("indexer not created!");
	std::map<const tchar*, const symbol*, stringless>::const_iterator iterfind = nameIndexer_.find(name.c_str());

	if(iterfind != nameIndexer_.end()) 
	{
		if(iterfind->second - get() != iterfind->second->sid)
			fire("error, sid not equal, %s, %d, %d", name.c_str(), (int)(iterfind->second - get()), iterfind->second->sid);
		return iterfind->second->sid;
	}
	//fire("can't find index of '%s'", name.c_str());
	return -1;
}
