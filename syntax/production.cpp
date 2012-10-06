#include "production.h"
#include <stringX.h>
#include <functionalX.h>

using namespace compile;

production::production()
{
}

production::production(const symholder_proxy& holder)
: holder_(holder)
{
}

production::production(int32 L, const int32* Rs, int32 N)
: L_(L)
, Rs_(Rs, N)
{
}

production::production(const symholder_proxy& holder, int32 L, const int32* Rs, int N)
: holder_(holder)
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

tstring production::to_string() const
{
	tstring tmp(holder_.at(L_).name);
	const tchar* sep = " -> ";
	foreach (int32 r, Rs_.begin(), Rs_.end())
	{
		tmp += sep;
		tmp += holder_.at(r).name;
		sep = " ";
	}
	return tmp;
}
