#include "eclosures.h"

using namespace compile;
using namespace compile::ga;

void eclosure::operator()(const tinygrammar& tig, closure_array& closures)
{
	const tinygrammar::vecprods& prods = tig.productions();
	const symholder& sholder = tig.symbols();

/*	kog::smart_vector<std::set<int32> > eClosures(sholder.size());
	for (size_t i = 0; i < sholder.size(); ++ i) 
		if(!sholder[i].ist) eClosures[i].insert(i);
	kog::smart_vector<int32> updated(sholder.size(), 1);
	while(std::find(updated.begin(), updated.end(), 1) != updated.end())
	{
		kog::smart_vector<int32> tmpv(updated.size(), 0);
		for(size_t i = 0; i < prods.size(); ++ i)
		{
			const int32 l = prods[i].left();
			const int32 r0 = prods[i].right().at(0);
			int osize = eClosures[l].size();
			if(ist[r0] && r0 != tig.eplisons() && r0 != tig.endings()) continue;
			else if(r0 == tig.endings() && prods[i].right_size() == 1)
				eClosures[l].insert(virtual_ending);
			else if(r0 == tig.eplisons())
			{
				if(prods[i].right_size() == 1) eClosures[l].insert(virtual_ending);
				else if(updated[prods[i].right()[1]])
				{
					int32 r1 = prods[i].right()[1];
					eClosures[l].insert(eClosures[r1].begin(), eClosures[r1].end());
				}
			}
			else if(updated[r0]) eClosures[l].insert(eClosures[r0].begin(), eClosures[r0].end());
			tmpv[l] = eClosures[l].size() != osize;
		}
		updated.swap(tmpv);
	}*/
}
