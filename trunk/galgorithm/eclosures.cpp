#include "eclosures.h"
#include "basicalgorithms.h"

#include <stack>

#include <scerror.h>

#ifdef DEBUG_OUTPUT
#include <logger.h>
#endif

using namespace compile;
using namespace compile::ga;

void eclosure::operator()(const tinygrammar& tig, closure_array& closures)
{
	const prodholder_proxy& prods = tig.productions();
	const symholder_proxy& sholder = tig.symbols();

	kog::smart_vector<int32> toe;
	int32 eid = -2;
	symbol_to_eplison stoe(tig, toe, eid);
	stoe.invoke();

	kog::smart_vector<std::set<int32> > edges(sholder.size());
	for (size_t i = 0; i < sholder.size(); ++ i) 
		if(!sholder[i].ist) edges[i].insert(i);
	// create graph
	int32 virtual_ending = -1;
	for (prodholder_proxy::const_iterator iter = prods.begin(); iter != prods.end(); ++ iter)
	{
		const int32 l = iter->left();
		const int32 r0 = iter->right().at(0);
		
		if(sholder[r0].ist && r0 != tig.eplisons() && r0 != tig.endings()) continue;
		else if(r0 == tig.endings())
		{
			if (iter->right_size() == 1) edges[l].insert(virtual_ending);
			else fire("invalidate grammar: ending!");
		}
		else
		{
			int32 x = 0;
			while(x < iter->right_size() && toe[iter->right()[x]])  edges[l].insert(iter->right()[x]);
			if(x < iter->right_size() && !sholder[iter->right()[x]].ist) edges[l].insert(iter->right()[x]);
		}
	}
	
	// depth first visist graph
	closures.reset(sholder.size());
	for (size_t i = 0; i < sholder.size(); ++ i)
	{
		if(sholder[i].ist) continue;
		std::stack<int32> visitstack;
		kog::smart_vector<int32> isvisited(sholder.size(), 0);
		visitstack.push(i);
		closure_array::value_type& iclosure = closures[i];
		while(!visitstack.empty())
		{
			int32 v = visitstack.top();
			visitstack.pop();
			isvisited[v] = 1;

			for (std::set<int32>::const_iterator iter = edges[v].begin(); iter != edges[v].end(); ++ iter)
			{
				if(!isvisited[*iter]) visitstack.push(*iter);
			}
		}

		int32 n = (int32)std::count(isvisited.begin(), isvisited.end(), 1);
		iclosure.reset(n);
		for (size_t v = 0, j = 0; v < sholder.size(); ++ v)
		{
			if(isvisited[v]) iclosure[j ++] = v;
		}

#ifdef DEBUG_OUTPUT
		logstring("eclosure[%s] = {", sholder[i].name);
		const char* sep = "";
		for(closure_array::value_type::const_iterator iterC = iclosure.begin();
			iterC != iclosure.end(); ++ iterC, sep = ", ")
			logstring("%s%s", sep, sholder[*iterC].name);
		logstring("}\n");
#endif
	}
}
