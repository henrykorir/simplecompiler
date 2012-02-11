#include "automachine.h"

using namespace sc;
using namespace compile;

automachine::automachine()
: cstate_(0)
, sstate_(0)
//, estate_(0)
, sheet_(new sparsesheet)
{
}

automachine::automachine(const shared_sheet& asheet)
: cstate_(0)
, sstate_(0)
//, estate_(0)
, sheet_(asheet)
{
}

automachine::~automachine()
{
}

void automachine::init()
{
	cstate_ = sstate_;
}

bool automachine::eta(int32 meta)
{
	const sheetrow& row = (*sheet_)[cstate_];
	sheetrow::const_iterator iterfind = row.end();
	switch(row.type())
	{
	case sheetrow::anychar :
		iterfind = row.begin();
		break;
	case sheetrow::special :
		iterfind = std::lower_bound(row.begin(), row.end(), 
			std::make_pair(meta, 0), intpair_less());
		break;
	case sheetrow::exclude:
		iterfind = std::lower_bound(row.begin(), row.end(), 
			std::make_pair(meta, 0), intpair_less());
		break;
	default:
		throw std::runtime_error("invalidate sheetrow in machine");
	}
	if(!((sheetrow::exclude == row.type()) ^ (iterfind != row.end() && iterfind->first == meta))) return false;
	cstate_ = iterfind->second;
	return true;
}
