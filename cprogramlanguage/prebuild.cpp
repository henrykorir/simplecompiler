#include "prebuild.h"

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
