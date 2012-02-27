#include "firstfollow.h"

#include <vector>
#include <set>
#include <list>

#ifdef DEBUG_OUTPUT
#include <logger.h>
#endif

using namespace compile;
using namespace compile::ga;

void firstset::operator()(const tinygrammar& tig, vecintset& sets)
{
	const symholder& sholder = tig.symbols();
	const tinygrammar::vecprods& prods = tig.productions();

	const size_t M = prods.size();
	const size_t N = sholder.size();

	std::vector<std::set<size_t> > TempSets(N);
	std::vector<int> IsVNEmpty(N);
	std::vector<int> IsProdUsed(M);
	std::vector<int> IsTerminate(N);

	size_t idxEmpty = -5;
	try{
		idxEmpty = sholder.index("");
	}catch(...){
	}
	for(size_t i = 0; i != N; ++ i)
	{
		const symbol& sym = sholder[i];
		if(sym.ist)
		{
			TempSets[i].insert(i);
			IsTerminate[i] = 1;
		}
	}

	std::vector<int> IsUpdated(N);
	for(size_t i = 0; i != M; ++ i)
	{
		const production& refP = prods[i];
		
		// A->aB...
		if(IsTerminate[refP.right().front()] != 0)
		{
			IsProdUsed[i] = 1;

			// A -> Empty
			if(refP.right_size() == 1 && refP.right().front() == idxEmpty)
			{
				IsVNEmpty[refP.left()] = 1;
			}
			else TempSets[refP.left()].insert(refP.right().front());
			IsUpdated[refP.left()] = 1;
		}
	}
	IsUpdated.assign(N, 1);

	std::vector<int> UpdateBuf(N);
	while(std::find(IsUpdated.begin(), IsUpdated.end(), 1) != IsUpdated.end())
	{
		UpdateBuf.assign(UpdateBuf.size(), 0);
		IsUpdated.assign(N, 1);
		for(size_t i = 0; i != M; ++ i)
		{
			if(IsProdUsed[i] != 0) continue;
			const production& refP = prods[i];
			production::right_array::const_iterator iter = refP.right().begin();
			size_t oldSize = TempSets[refP.left()].size();
			for(;iter != refP.right().end(); ++ iter)
			{
				if(IsUpdated[*iter] != 0)
				{
					TempSets[refP.left()].insert(
						TempSets[*iter].begin(), TempSets[*iter].end());
				}

				if(IsVNEmpty[*iter] == 0)
				{
					break;
				}
			}
			UpdateBuf[refP.left()] |= oldSize != TempSets[refP.left()].size() ? 1 : 0;
			if(iter == refP.right().end() && IsVNEmpty[refP.left()] == 0) // A->Empty
			{
				IsVNEmpty[refP.left()] = 1;
				UpdateBuf[refP.left()] |= 1;
			}
		}
		UpdateBuf.swap(IsUpdated);
	}

	sets.reset(N);
	for(size_t i = 0; i != N; ++ i)
	{
		sets[i].reset(TempSets[i].size() + (IsVNEmpty[i] != 0 ? 1 : 0));
		std::copy(TempSets[i].begin(), TempSets[i].end(), sets[i].begin());
		if(IsVNEmpty[i]) sets[i].back() = -1; // virtual ending
	}
#ifdef DEBUG_OUTPUT
	logstring("\nFirst set!\n");
	for (size_t i = 0; i < N; ++ i)
	{
		if (IsTerminate[i]) continue;
		logstring("first(%s) = {", sholder[i].name);
		const char* sep = "";
		for (size_t j = 0; j < sets[i].size(); ++ j, sep = ", ")
		{
			const char* name = sets[i][j] == -1 ? "#" : sholder[sets[i][j]].name;
			logstring("%s%s", sep, name);
		}
		logstring("}\n");
	}
#endif
}

void followset::operator()(const tinygrammar& tig, const vecintset& FirstSets, vecintset& FollowSets)
{
	typedef std::vector<std::list<size_t> > UIntListVector; 
	const symholder& sholder = tig.symbols();
	const tinygrammar::vecprods& prods = tig.productions();
	const size_t M = prods.size();
	const size_t N = sholder.size();

	std::vector<std::set<size_t> > TempSets(N);
	std::vector<int> IsVNEmpty(N);
	std::vector<int> IsProdUsed(M);
	std::vector<int> IsTerminate(N);

	size_t idxEmpty = sholder.index("");
	size_t idxStart = tig.starts();
	int32 idxEnd = tig.endings();
	for(size_t i = 0; i != N; ++ i)
	{
		const symbol& sym = sholder[i];
		IsTerminate[i] = sym.ist;
	}

	
	for(size_t i = 0; i != N; ++ i)
	{
		if(std::find(FirstSets[i].begin(), FirstSets[i].end(), idxEmpty) != FirstSets[i].end())
			IsVNEmpty[i] = 1;
	}

	TempSets[idxStart].insert(idxEnd);
	for(size_t i = 0; i != M; ++ i)
	{
		const production& refProd = prods[i];
		for(production::right_array::const_iterator iter = refProd.right().begin(),
			iter_next = kog::iterator_next(iter);
			iter_next != refProd.right().end(); ++ iter, ++ iter_next)
		{
			if(IsTerminate[*iter]) continue;
			TempSets[*iter].insert(FirstSets[*iter_next].begin(),
				FirstSets[*iter_next].end());
			TempSets[*iter].erase(idxEmpty);
		}
	}
	std::vector<int> IsUpdated(N, 1);

	std::vector<int> UpdateBuf(N);
	while(std::find(IsUpdated.begin(), IsUpdated.end(), 1) != IsUpdated.end())
	{
		UpdateBuf.assign(UpdateBuf.size(), 0);
		
		for(size_t i = 0; i != M; ++ i)
		{
			const production& refProd = prods[i];
			if(IsUpdated[refProd.left()] == 0) continue;
			for(production::right_array::const_reverse_iterator iter = refProd.right().rbegin();
				iter != refProd.right().rend(); ++ iter)
			{
				if(IsTerminate[*iter]) break;
				size_t oldSize = TempSets[*iter].size();
				TempSets[*iter].insert(TempSets[refProd.left()].begin(),
					TempSets[refProd.left()].end());
				UpdateBuf[*iter] |= TempSets[*iter].size() != oldSize ? 1 : 0;
				if(IsVNEmpty[*iter] == 0) break;
			}
		}

		UpdateBuf.swap(IsUpdated);
	}

	FollowSets.reset(N);
	for(size_t i = 0; i != N; ++ i)
	{
		FollowSets[i].reset(TempSets[i].size());
		std::copy(TempSets[i].begin(), TempSets[i].end(), FollowSets[i].begin());
	}
}
