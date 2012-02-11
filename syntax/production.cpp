#include "production.h"

using namespace compile;

production::production()
: holder_(NULL)
{
}

production::production(const symholder& holder)
: holder_(&holder)
{
}

production::production(int32 L, const int32* Rs, int32 N)
: holder_(NULL)
, L_(L)
, Rs_(Rs, N)
{
}

production::production(const symholder& holder, int32 L, const int32* Rs, int N)
: holder_(&holder)
, L_(L)
, Rs_(Rs, N)
{
}

void production::reset_right(const int32* Rs, int N)
{
	kog::smart_vector<int32> tmp(Rs, N);
	Rs_.swap(tmp);
}

void production::swap(production& other)
{
	if(this == &other) return;
	std::swap(L_, other.L_);
	Rs_.swap(other.Rs_);
	std::swap(holder_, other.holder_);
}
