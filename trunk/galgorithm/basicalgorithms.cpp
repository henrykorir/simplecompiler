#include "basicalgorithms.h"

#include <vector>
#include <list>
#include <deque>
#include <stack>
#include <functionalX.h>
#include <stringX.h>

#define DEBUG_OUTPUT
#ifdef DEBUG_OUTPUT
#include <iostream>
#endif

using namespace compile;
using namespace compile::ga;

void removenotused::operator()(const grammar& gin, grammar& gout)
{
	const tinygrammar& tig = gin.gettinyg();
	useds.reset(tig.symbols().size());
	usedp.reset(tig.productions().size());
	memset(useds.get(), 0, useds.size_in_bytes());
	memset(usedp.get(), 0, usedp.size_in_bytes());

	// stage 1: terminate test
	rm_notoT(tig);
	// stage 2: start_symbol test
	rm_Snofm(tig);

	// new grammar 
	new_grammar(gin, gout);
}

void removenotused::rm_notoT(const tinygrammar& tig)
{
	for(size_t i = 0; i != useds.size(); ++ i)
	{
		useds[i] = tig.symbols()[i].ist != 0;
	}

	size_t nLastCount = std::count(useds.begin(), useds.end(), 1);
	size_t nNewCount = nLastCount;
	
	const tinygrammar::vecprods& refProductions = tig.productions();
	do{
		nLastCount = nNewCount;
		for(size_t i = 0; i != refProductions.size(); ++ i)
		{
			const production& refProd = refProductions[i];
			if(!usedp[i])
			{
			//	if(!useds[refProd.left()])
				{
					production::right_array::const_iterator iter = refProd.right().begin();
					production::right_array::const_iterator iter_end = refProd.right().end();
					for(iter; iter != iter_end && useds[*iter]; ++ iter);
					if(iter == iter_end)
					{
						useds[refProd.left()] = 1;
						usedp[i] = 1;
					}
				}
			}
		}
		nNewCount = std::count(useds.begin(), useds.end(), 1);
	}while(nNewCount != nLastCount);

#ifdef DEBUG_OUTPUT
	std::cout<<"can to terminate:\n";
	for(size_t i = 0; i < useds.size(); ++ i)
	{
		if(useds[i]) std::cout<<tig.symbols()[i].name<<"\n";
	}
#endif
}

void removenotused::rm_Snofm(const tinygrammar& tig)
{
	useds[tig.starts()] |= 0x02;

	size_t nLastCount = std::count(useds.begin(), useds.end(), 0x03);
	size_t nNewCount = nLastCount;
	
	const tinygrammar::vecprods& refProductions = tig.productions();
	do{
		nLastCount = nNewCount;
		for(size_t i = 0; i != refProductions.size(); ++ i)
		{
			const production& refProd = refProductions[i];
			if((usedp[i] == 1) && useds[refProd.left()] == 0x03)
			{
				production::right_array::const_iterator iter = refProd.right().begin();
				production::right_array::const_iterator iter_end = refProd.right().end();
				for(iter; iter != iter_end; ++ iter)
					useds[*iter] |= 0x02;
				usedp[i] |= 0x02;
			}
		}
		nNewCount = std::count(useds.begin(), useds.end(), 0x03);
	}while(nNewCount != nLastCount);
	// reset usedp
	for(size_t i = 0; i < usedp.size(); ++ i)
		usedp[i] = usedp[i] == 3 ? 1 : 0;
	for(size_t i = 0; i < useds.size(); ++ i)
		useds[i] = useds[i] == 3 ? 1 : 0;

#ifdef DEBUG_OUTPUT
	std::cout<<"can from start:\n";
	for(size_t i = 0; i < useds.size(); ++ i)
	{
		if(useds[i]) std::cout<<tig.symbols()[i].name<<"\n";
	}
#endif
}

void removenotused::new_grammar(const grammar& gin, grammar& gout)
{
	const tinygrammar& tig = gin.gettinyg();
	size_t nNewCount = std::count(useds.begin(), useds.end(), 1);
	std::vector<symbol> NewSymbols(nNewCount);
	std::vector<int> OldNewMap(useds.size());
	for(size_t i = 0, j = 0; i < useds.size(); ++ i)
	{
		if(useds[i]) 
		{
			OldNewMap[i] = (int)j;
			NewSymbols[j++] = tig.symbols()[i];
		}
	}

	const tinygrammar::vecprods& refProductions = tig.productions();
	std::list<production> prodList;
	for(size_t i = 0; i < usedp.size(); ++ i)
	{
		if(!usedp[i]) continue;
		const production& refProd = refProductions[i];
		production::right_array tempRightList(refProd.right_size());
		production::right_array::const_iterator iterS = refProd.right().begin();
		for(production::right_array::iterator iterD = tempRightList.begin();
			iterD != tempRightList.end(); ++ iterD, ++ iterS)
		{
			*iterD = OldNewMap[*iterS];
		}
		prodList.push_back(production(OldNewMap[refProd.left()], tempRightList.get(), tempRightList.size()));
	}

	tinygrammar gout_kernel(NewSymbols.begin(), NewSymbols.end(), prodList.begin(), prodList.end(), OldNewMap[tig.starts()]);
	grammar tmp(gout_kernel);
	gout.swap(tmp);
}

void removesingle::operator()(const grammar& gin, grammar& gout)
{
	const tinygrammar& tig = gin.gettinyg();

	const tinygrammar::vecprods& refProductions = tig.productions();
	typedef kog::smart_vector<int32> vecint;

	vecint IsTerm(tig.symbols().size());

	const size_t M = refProductions.size();
	const size_t N = tig.symbols().size();
	for(size_t i = 0; i != N; ++ i)
	{
		IsTerm[i] = tig.symbols()[i].ist != 0;
	}
	
	// init
	//typedef kog::smart_vector<kog::smart_vector<int32> > smart_matrix;
	//smart_matrix Wset(N, kog::smart_vector<int32>(N, 0));
	std::list<production> NewProdList;
	std::vector<std::pair<int, int> > singleP;
	for(size_t i = 0; i < M; ++ i)
	{	
		const production& p = refProductions[i];
		if(p.right_size() == 1 && IsTerm[p.right()[0]] == 0)
		{
			singleP.push_back(std::make_pair(p.left(), p.right()[0]));
		}
		else NewProdList.push_back(p);
	}
	std::sort(singleP.begin(), singleP.end(), intpair_less()); //sort by first
	std::vector<int> idx(N, -1);
	for(size_t i = 0; i < singleP.size(); ++ i)
	{
		if(idx[singleP[i].first] == -1) idx[singleP[i].first] = i;
	}
	singleP.push_back(std::make_pair(-1, -1)); // to make for(v=idx[x];;) to stop 

	for(size_t i = 0; i != N; ++ i)
	{
		if(IsTerm[i] == 1 || idx[i] == -1) continue;
		std::deque<int32> Wstack;
		kog::smart_vector<int32> Wi(N);
		memset(Wi.get(), 0, Wi.size_in_bytes());
		Wstack.push_back(i);
		while(!Wstack.empty())
		{
			int32 x = Wstack.back();
			Wstack.pop_back();
			if(Wi[x]) continue;
			Wi[x] = 1;
			
			for(size_t v = idx[x]; singleP[v].first == x; ++ v) 
				Wstack.push_back(singleP[v].second);
		}

		Wi[i] = 0; // set A -> A not occure
	
		for(size_t j = 0; j < M; ++ j)
		{
			const production& p = refProductions[j];
			if(!(Wi[p.left()] && (p.right_size() != 1 || IsTerm[p.right()[0]]))) continue;
			NewProdList.push_back(production(i, p.right().get(), p.right_size()));
		}
	}
	// remove duplicate productions
	//NewProdList.unique();
	remove_duplicate(NewProdList);

	tinygrammar totg(tig.symbols().begin(), tig.symbols().end(), NewProdList.begin(), NewProdList.end(), tig.starts());
	grammar tmp(totg);
	gout.swap(tmp);
}

void removesingle::remove_duplicate(std::list<production>& plist)
{
	plist.unique(p_equal());
}

void eliminate_eplison::operator()(const grammar& gin, grammar& gout)
{
	const tinygrammar& tig = gin.gettinyg();

	const symholder& sholder = tig.symbols();
	const tinygrammar::vecprods& prods = tig.productions();

	toe.reset(sholder.size());
	memset(toe.get(), 0, toe.size_in_bytes());
	
	const tchar* eplison = ""; // eplison is empty string
	eid = sholder.index(eplison);
	// eplison in G(Vn, Vt, P, S)?
	if(-1 == eid) 
	{
		grammar tmpG(gin);
		gout.swap(tmpG);
		return;
	}

	findtoe(tig);

	tinygrammar tog;
	if(toe[tig.starts()]) // eplison in G
	{
		if(is_start_in_right(tig))
		{
			new_start_symbol(tig, tog);
			rmeplison(tog, tog);
		}
		else rmeplison(tig, tog);
	}
	else rmeplison(tig, tog); // remove eplison symbol

	grammar tmp(tog);
	tmp.swap(gout);
}

void eliminate_eplison::findtoe(const tinygrammar& tig)
{
	const symholder& sholder = tig.symbols();
	const tinygrammar::vecprods& prods = tig.productions();
	
	kog::smart_vector<int32> IsProdUsed(prods.size());
	memset(IsProdUsed.get(), 0, IsProdUsed.size_in_bytes());

	toe[eid] = 1;

	size_t nLastCount = 1;
	size_t nNewCount = 1;

	do{
		nLastCount = nNewCount;
		for(size_t i = 0; i < prods.size(); ++ i)
		{
			const production& p = prods[i];
			if(toe[p.left()] || IsProdUsed[i]) continue;
			production::right_array::const_iterator iter = p.right().begin();
			production::right_array::const_iterator iter_end = p.right().end();
			for(; iter != iter_end; ++ iter)
			{
				if(!toe[*iter]) break;
			}
			if(iter_end == iter)
			{
				toe[p.left()] = 1;
				IsProdUsed[i] = 1;
			}
		}
		
		nNewCount = std::count(toe.begin(), toe.end(), 1);
	}while(nNewCount != nLastCount);

#ifdef DEBUG_OUTPUT
	std::cout<<"Wset = { ";
	for(size_t i = 0, j = 0; i < toe.size(); ++ i)
	{
		if(toe[i] && i != eid) 
		{
			if(j ++ != 0) std::cout<<", ";
			std::cout<<tig.symbols()[i].name;
		}
	}
	std::cout<<" }"<<std::endl;
#endif
}

bool eliminate_eplison::is_start_in_right(const tinygrammar& tig) const
{
	const int32 ssid = tig.starts();
	const tinygrammar::vecprods& prods = tig.productions();
	for(size_t i = 0; i < prods.size(); ++ i)
	{
		const production& p = prods[i];

		production::right_array::const_iterator iter = p.right().begin();
		production::right_array::const_iterator iter_end = p.right().end();
		while(iter != iter_end && *iter != ssid) ++ iter;
		if(iter != iter_end) return true;
	}
	return false;
}

void eliminate_eplison::new_start_symbol(const tinygrammar& tig, tinygrammar& tog)
{
	const int32 ssid = tig.starts();
	const symholder& sholder = tig.symbols();
	std::deque<symbol> new_symbols(sholder.begin(), sholder.end());
	const tinygrammar::vecprods& prods = tig.productions();
	symbol asym;
	asym.Lfuncs = 0;
	asym.ist = 0;
	asym.sid = sholder.size();
	std::string name_tmp = stringX::format("S%d", asym.sid);
	asym.name = name_tmp.c_str();
	asym.Lname = (int32)name_tmp.size();

	new_symbols.push_back(asym);

	std::deque<production> new_prods(prods.begin(), prods.end());
	for(size_t i = 0; i < new_prods.size(); ++ i)
	{
		if(prods[i].left() == ssid && !(prods[i].right_size() == 1 && prods[i].right()[0] == eid))
			new_prods.push_back(production(asym.sid, prods[i].right().get(), prods[i].right_size()));
	}

	tinygrammar tmp(new_symbols.begin(), new_symbols.end(), new_prods.begin(), new_prods.end(), asym.sid);
	tmp.swap(tog);

	// make new toe set
	kog::smart_vector<int32> tmptoe(toe.size() + 1);
	std::copy(toe.begin(), toe.end(), tmptoe.begin());
	tmptoe.back() = toe[tig.starts()];
	tmptoe.swap(toe);
}

void eliminate_eplison::rmeplison(const tinygrammar& tig, tinygrammar& tog)
{
	const int32 ssid = tig.starts();
	const symholder& sholder = tig.symbols();
	const tinygrammar::vecprods& prods = tig.productions();

	kog::smart_vector<int32> oldnewmap(sholder.size());
	if(toe[ssid]) for(size_t i = 0; i < oldnewmap.size(); ++ i) oldnewmap[i] = (int32)i;
	else
	{
		for(size_t i = 0; i < eid; ++ i) oldnewmap[i] = (int32)i;
		for(size_t i = eid+1; i < sholder.size(); ++ i) oldnewmap[i] = (int32)i - 1;
		oldnewmap[eid] = -1;
	}

	std::list<production> NewProductions;
	for(size_t i = 0; i != prods.size(); ++ i)
	{
		const production& p = prods[i];

		if(p.right_size() == 1 && p.right()[0] == eid)// A->empty
			continue;
		else
		{
			typedef kog::triple<size_t, int, production::right_array::const_iterator> stack_elem;
			std::stack<stack_elem> SymStack;
			SymStack.push(kog::make_triple(
						p.right().front(), toe[p.right().front()], kog::iterator_next(p.right().begin())));
			std::vector<int32> tmp_right;
			tmp_right.push_back(p.right().front());
			while(!SymStack.empty())
			{
				stack_elem top = SymStack.top();
				if(top.third == p.right().end())
				{
					if(tmp_right.size() && !(tmp_right.size() == 1 && tmp_right[0] == eid))
					{
						kog::smart_vector<int32> rtmp(tmp_right.size());
						for(size_t i = 0; i < rtmp.size(); ++ i) rtmp[i] = oldnewmap[tmp_right[i]];
						NewProductions.push_back(production(oldnewmap[p.left()], rtmp.get(), rtmp.size()));
					}
					while(!SymStack.empty() && SymStack.top().second == 0)
					{
						if(!toe[SymStack.top().first]) tmp_right.pop_back();
						SymStack.pop();
					}

					if(!SymStack.empty()) 
					{
						SymStack.top().second = 0;
						tmp_right.pop_back();
					}
				}
				else
				{
					size_t x = *top.third;
					SymStack.push(kog::make_triple(x, toe[x], kog::iterator_next(top.third)));
					tmp_right.push_back(x);
				}
			}
		}
	}

	if(toe[tig.starts()])
	{
		// add production Start->eplison
		NewProductions.push_back(production(oldnewmap[tig.starts()], &oldnewmap[eid], 1));
		tinygrammar tmp(sholder.begin(), sholder.end(), NewProductions.begin(), NewProductions.end(), tig.starts());
		tog.swap(tmp);
	}
	else // remove eplison symbol
	{
		std::deque<symbol> newsymlist(sholder.size() - 1);
		for(size_t i = 0, j = 0; i < sholder.size(); ++ i)
			if(i != eid) newsymlist[j ++] = sholder[i];
		tinygrammar tmp(newsymlist.begin(), newsymlist.end(), NewProductions.begin(), NewProductions.end(), oldnewmap[tig.starts()]);
		tog.swap(tmp);
	}
}

void simplegrammar::operator()(const grammar& gin, grammar& gout)
{
	grammar gtmp;
	// remove eplison
	eliminate_eplison rme(gin, gtmp);
	// remove single production
	removesingle rms(gtmp, gtmp);
	// remove not used symbols and productions
	removenotused rmnu(gtmp, gtmp);
	
	gtmp.swap(gout);
}
