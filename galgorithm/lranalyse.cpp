#include "lranalyse.h"

#include <vector>
#include <list>
#include <deque>
#include <map>

#include <functionalX.h>
#include <sortc.h>

#include "eclosures.h"
#include "firstfollow.h"

using namespace compile;
using namespace compile::ga;
/*(
typedef kog::sortc<std::vector<int32>, std::less<int32> > followset;

struct lrstateitem
{
	const production* prod; // ref production
	int32 dot; // dot position
	followset follow;
};

class lrstate : public std::vector<lrstateitem>
{
//	automachine::sheetrow rows;
};

struct nextstate
{
	nextstate(const lrstate* p)
		: ps(p), sorp(0)
	{}

	nextstate(const production* p)
		: pp(p), sorp(1)
	{}

	union 
	{
		const lrstate* ps;
		const production* pp;
	};
	int32 sorp; // 0: s, 1: p
};

struct AlgorithmArg
{
	AlgorithmArg(const grammar& orggrammar);
	std::vector<int> IsVNEmpty;
	std::vector<int> IsTerminate;
	kog::smart_vector<const production*> plist;
	kog::smart_vector<int32> ntpi; 
	std::list<lrstate> lrsts;
	std::list<kog::triple<const lrstate*, int32, nextstate> > sparsesheet;
	std::vector<size_t> productionMap;
	 FirstSets;
	const tinygrammar& tig;

	void update_closure(lrstate& lrs) const;
	void update_firsts(lrstateitem& snew, const lrstateitem& sorg) const;
};

void lranalyse::operator()(const grammar& gin, lrmachine& mot) const
{
	AlgorithmArg arg(gin.gettinyg());

	std::queue<lrstate*> sQueue;
	sQueue.push(&arg.lrsts.front());
	
	while(!sQueue.empty())
	{
		const lrstate* pRow = sQueue.front();
		sQueue.pop();
	
		// reduce - reduce conflict
		// reduce - shiftin conflict
		arg.CheckLR1(pRow);

		typedef std::pair<int32, const lrstateitem*> etapair;
		std::pair<etapair> eats;// first: next symbol, second: ref lrstateitem
		for(lrstate::const_iterator iter = pRow->begin(); iter != pRow->end(); ++ iter)
		{
			const production& p = *iter->prod;
			if(iter->dot == p.right_size()) // reduce item
			{
				// using first(By) to make reduce item
				for(std::set<int32>::const_iterator iterf = iter->follow.begin(); iterf != iter->follow.end(); ++ iterf)
				{
					//reduces.push_back(std::make_pair(*iter, &p));
					arg.sparsesheet.push_back(kog::make_triple(&(*iter), *iterf, nextstate(&p)));
				}
			}
			else
			{
				eats.push_back(kog::make_pair(p.right()[iter->dot], &(*iter)));
			}
		}
		// sort by first 
		std::sort(eats.begin(), eats.end(), kog::composite_function(kog::mem_value(&etapair::first),
					kog::mem_value(&etapair::first), std::less<int32>()));

		for(size_t i = 0, j = 0; i < eats.size(); i = j)
		{
			// [i, j)
			lrstate s;
			for(j = i; j < eats.size() && eats[i].first == eats[j].first; ++ j)
			{
				lrstateitem item;
				item.prod = etas[j].second->prod;
				item.dot = etas[j].second->dot + 1;
				s.pushback(item);
				// update follow set
				arg.update_follow(item, etas[j].second->follow)
			}
			arg.upate_closure(s);
			if((const lrstate* pNewS = arg.insert_new_state(pRow, etas[i].first, &s)) != NULL)
				sQueue.push(pNewS);
		}
	}

	automachine::shared_sheet psheet(new automachine::sparsesheet(arg.lrsts.size() + 1)); // s0 is ending status
	automachine::sparsesheet& sheet = *psheet();
	typedef kog::triple<const lrstate*, int32, nextstate> gototriple;
	arg.sparsesheet.sort(kog::composite_function(kog::mem_value(&gototriple::first),
					kog::mem_value(&gototriple::first), std::less<const lrstate*>()));
	for (gototriple::const_iterator iterI = arg.sparsesheet.begin(), iterJ; iterI != arg.sparsesheet.end(); )
	{
	}
}

size_t Algorithm_IsLR1::AlgorithmArg::UpdateClosure(LR::LRState& orgState) const
{
	const size_t P = orgState.size();
	std::list<LR::Closure> TempClosures(P);
	std::list<LR::Closure>::iterator iter_temp = TempClosures.begin();
	std::queue<std::list<LR::Closure>::iterator> Q;
	std::list<size_t> EmptySet;
	for(size_t i = 0; i != P; ++ i)
	{
		iter_temp->swap(orgState[i]);
		Q.push(iter_temp);
		++ iter_temp;
	}

	while(!Q.empty())
	{
		std::list<LR::Closure>::iterator iter = Q.front();
		Q.pop();
		right_range range = GetNextSymbols(iter->ProdutionNo(), iter->DotPos());
		if(range.first != range.second && !IsTerminate[*range.first])
		{// non-terminate
			std::list<size_t> NewFollows;
			right_range yrange = range;
			++ yrange.first;
			if(GetFirsts(yrange, NewFollows))
			{
				NewFollows.insert(NewFollows.begin(), iter->Follows().begin(), iter->Follows().end());
			}
			
			// add all A->.b
			std::vector<grammar::RefProduction>::const_iterator iterProd = 
				std::lower_bound(refProductions.begin(), refProductions.end(),
				std::make_pair(*range.first, std::list<size_t>()), ProductionCmpLeft());
			size_t idx = std::distance(refProductions.begin(), iterProd);
			while(iterProd != refProductions.end() && iterProd->first == *range.first)
			{
				LR::Closure tempClosure(idx, 0, NewFollows.begin(), NewFollows.end());
				// if found the same closure
				std::list<LR::Closure>::iterator iterSameClosure = 
					std::find_if(TempClosures.begin(), TempClosures.end(),
					//std::bind2nd(LR::Closure::ClosureKernelEqual(), tempClosure));
					std::bind2nd(LR::Closure::ClosureEqual(), tempClosure));
				if(iterSameClosure == TempClosures.end())
				{// can't find a same closure
					TempClosures.push_back(tempClosure);
					//TempClosures.back().swap(LR::Closure(idx, 0, NewFollows.begin(), NewFollows.end()));
					//Q.push(TempClosures.rbegin().base());
					std::list<LR::Closure>::iterator iter_n = TempClosures.rbegin().base();
					Q.push(-- iter_n);
				}
				else
				{// 
					//throw std::exc
					//std::list<size_t> CombinedFollows(iterSameClosure->Follows().begin(),
					//	iterSameClosure->Follows().end());
					//CombinedFollows.insert(CombinedFollows.end(), 
					//	NewFollows.begin(), NewFollows.end());
					//iterSameClosure->swap(LR::Closure(iterSameClosure->ProdutionNo(), iterSameClosure->DotPos(),
					//	CombinedFollows.begin(), CombinedFollows.end()));
				}
				++ iterProd;
				++ idx;
			}
		}
	}

	LR::Closure::ClosureLess cLess;
	TempClosures.sort(cLess);
	// combine
	std::vector<int> IsStatute;
	IsStatute.reserve(TempClosures.size());
	for(std::list<LR::Closure>::iterator iter_temp = TempClosures.begin();
		iter_temp != TempClosures.end(); )
	{
		std::list<LR::Closure>::iterator iter_temp2 = iter_temp;
		++ iter_temp2;
		for(; iter_temp2 != TempClosures.end();)
		{
			if(iter_temp2->ProdutionNo() == iter_temp->ProdutionNo() && 
				iter_temp2->DotPos() == iter_temp->DotPos())
			{
				iter_temp->Follows().insert(iter_temp->Follows().end(), iter_temp2->Follows().begin(),
					iter_temp2->Follows().end());
				iter_temp2 = TempClosures.erase(iter_temp2);
			}
			else break;
		}
		iter_temp->Follows().erase(
			std::unique(iter_temp->Follows().begin(), iter_temp->Follows().end()),
			iter_temp->Follows().end());
		IsStatute.push_back(
			iter_temp->DotPos() == 
			refProductions[iter_temp->ProdutionNo()].second.size() ? 1 : 0);
		iter_temp = iter_temp2;
	}

	orgState.swap(LR::LRState(TempClosures.begin(), TempClosures.end()));
	
	size_t idx = 0;
	LR::LRState::IsLRStateEqual sEqual;
	for(std::list<LR::AnalysisSheet::SheetRow>::const_iterator iter = sheetRows.begin();
		iter != sheetRows.end(); ++ iter, ++ idx)
	{
		if(sEqual(iter->first, orgState)) return idx;
	}

#ifdef SHOW_DEBUG_MESSAGE
	std::clog<<"I"<<sheetRows.size()<<"\n";
	for(size_t i = 0; i != orgState.size(); ++ i)
	{
		LR::Closure& item = orgState[i];
		const grammar::RefProduction& prod = refProductions[item.ProdutionNo()];
		if(prod.first >= grammar.GetSymbolCount())
			std::clog<<"S1"<<" -> ";
		else std::clog<<grammar.GetSymbol(prod.first)->Name()<<" -> ";
		std::list<size_t>::const_iterator iter_r = prod.second.begin();
		for(size_t j = 0; j != prod.second.size(); ++ j, ++ iter_r)
		{
			if(item.DotPos() == j) std::clog<<".";
			std::clog<<grammar.GetSymbol(*iter_r)->Name();
		}
		if(item.DotPos() == prod.second.size()) std::clog<<".";
		std::clog<<"\t,";
		for(LR::Closure::FollowVector::const_iterator iter_f = item.Follows().begin(); iter_f != item.Follows().end(); ++ iter_f)
		{
			Compile::_Str str = grammar.GetSymbol(*iter_f)->Name();
			if(str == Symbol::EndSymbol) str = "#";
			std::clog<<str;
		}
		std::clog<<"\n";
	}
	std::clog<<"\n";
#endif

	return -1;
}

bool AlgorithmArg::CheckLR1(const automachine::sheetrow& sortedActs) const
{
	if(!sortedActs.empty())
	{
		automachine::sheetrow::const_iterator iterF = sortedActs.begin();
		automachine::sheetrow::const_iterator iterN = algfun::iterator_next(iterF);
		automachine::sheetrow::const_iterator iter_end = sortedActs.end();
		while(iterN != iter_end && iterF->first != iterN->first) ++ iterF, ++ iterN;
		return iterN == iter_end;
	}
	return true;
}

void Algorithm::update_closure(lrstate& li)
{
	std::set<int32> cset;
	const prodholder& pholder = tig.productions();
	const symholder& sholder = tig.symbols();
	
	kog::smart_vector<followset*> pfollows(pholder.size(), NULL);
	size_t nlastsize = 0;

	for (lrstate::const_iterator iter = li.begin(); iter != lr.end(); ++ iter)
	{
		const production& p = *iter->prod;
		pfollows[(int32)(iter->prod - pholder.get())] = new followset(iter->follow);
		if(p.right_size() != iter->dot && !sholder[p.right()[iter->dot]].ist) // not last symbol?
		{
			cset.insert(sholder[p.right()[iter->dot]]);
		}
	}

	for (std::set<int32>::const_iterator iter = cset.begin(); iter != cset.end(); ++ iter)
	{
		for (size_t i = npti[*iter]; i < plist.size() && plist[i].left() == *iter; ++ i)
		{
			if(ispused[i])  // update follows
			{
			}
			else
			{
				ispused[i] = 1;
				lrstateitem itm;
				itm.prod = plist[i];
				itm.dot = 0;
				// get follows of new item
				li.push_back(itm);
			}
		}
	}
}

// A -> aB
void Algorithm::insert_new_state(const lrstate* A, int32 a, const lrstate* B)
{
	const lrstate* ps = NULL;
	if((std::list<lrstate>::iterator iterfind = std::find(lrsts.begin(), lrsts.end(), *B)) != lrsts.end())
	{
		// find an existing lrstate
		ps = &(*iterfind);
	}
	else
	{
		lrsts.push_back(*B);
		ps = &lrsts.back();
	}
	sparsesheet.push_back(kog::make_triple(A, a, ps));
}

void AlgorithmArg::get_firsts(int32 x, const followset& f, followset& nf) const
{
	if (x == -1) // end of production
	{
		nf.insert(f.begin(), f.end());
	}
	else if(ists[x])
	{
		nf.insert(x);
	}
	else nf.insert(firstsets[x].begin(), firstsets[x].end());
}

int32 AlgorithmArg::get_next_symbol(const production& p, int32 idot) const
{
	if(p.right_size() == idot) return -1; // ending of production
	return p.right[idot];
}

AlgorithmArg::AlgorithmArg(const grammar& orggrammar)
: IsVNEmpty(orggrammar.GetSymbolCount() + 1)
, IsTerminate(orggrammar.GetSymbolCount() + 1)
, refProductions(orggrammar.GetProductionCount() + 1)
, productionMap(orggrammar.GetProductionCount() + 1)
, grammar(orggrammar)
{
	const size_t M = orggrammar.GetProductionCount();
	const size_t N = orggrammar.GetSymbolCount();

	kog::smart_vector<const production*> plist(pholder.size());
	std::transform(pholder.begin(), pholder.end(), plist.begin(), kog::get_ptr_t<production>());
	// sort by production left
	kog::depointer_t<const production> dpp;
	std::sort(plist.begin(), plist.end(), kog::composite_function(dpp, dpp, pleft_less()));
	kog::smart_vector<int32> ntpi(sholder.size(), -1);
	for(size_t i = 0; i < plist.size(); ++ i)
		if(ntpi[plist[i]->left()] == -1) ntpi[plist[i]->left()] = i;

	// get e-closure of every symbol
	eclosures::closure_array closures;
	eclosures ec_algorithm(tig, closures);
	ec_algorithm.invoke();

	size_t idxEmpty = -1;
	size_t idxStart = -1;
	size_t idxEnd = -1;
	for(size_t i = 0; i != N; ++ i)
	{
		const Symbol* pSym = orggrammar.GetSymbol(i);
		IsTerminate[i] = pSym->IsTerminate();
		if(pSym->IsEmpty()) idxEmpty = i;
		else if(pSym->Content() == orggrammar.GetStartSymbol())
			idxStart = i;
		else if(pSym->IsEnd()) idxEnd = i;
	}
	if(idxEmpty < N) IsVNEmpty[idxEmpty] = 1;
	// Symbol[N] = S';

	refProductions.reserve(M + 1);
	orggrammar.GetAllRefProduction(refProductions);
	// S' -> S
	refProductions.push_back(grammar::RefProduction(N, std::list<size_t>(1, idxStart)));
	//std::sort(refProductions.begin(), refProductions.end(), ProductionCmpLeft());
	//std::vector<std::pair<size_t, size_t> > 
	//for(size_t i = 0; i != M + 1; ++ i)
	//	productionMap[i] = i;
	//std::sort(
	productionMap = algfun::index_sort(refProductions.begin(), refProductions.end(),
		ProductionCmpLeft());

	Algorithm_First algorithmFirst;
	//FirstSets.reserve(N + 1);
	if(0 != algorithmFirst(orggrammar, FirstSets))
		throw std::exception("can't calculate first sets of the grammar!");

	for(size_t i = 0; i < N; ++ i)
	{
		FirstSets[i].sort();
		if(std::find(FirstSets[i].begin(), FirstSets[i].end(), idxEmpty) != FirstSets[i].end())
			IsVNEmpty[i] = 1;
	}

	std::vector<size_t> buf(1, idxEnd);
	LR::Closure item(M, 0, buf.begin(), buf.end());
	LR::LRState tempState(&item, &item + 1);
	UpdateClosure(tempState);
	sheetRows.push_back(std::make_pair(tempState, automachine::sheetrow()));
	//UpdateClosure(sheetRows.front().first);
}
*/