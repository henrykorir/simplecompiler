#include "lranalyse.h"

#include <vector>
#include <list>
#include <deque>
#include <queue>
#include <map>
#include <iostream>
#include <memory>

#include <functionalX.h>
#include <sortc.h>
#include <scerror.h>

#include "eclosures.h"
#include "firstfollow.h"

#ifdef DEBUG_OUTPUT
#include <logger.h>
#endif

using namespace compile;
using namespace compile::ga;

typedef kog::sortc<std::vector<int32>, std::less<int32> > sfollowset;

struct lrstateitem
{
	const production* prod; // ref production
	int32 dot; // dot position
	sfollowset follow;

	bool operator == (const lrstateitem& other) const
	{
		if(prod != other.prod || dot != other.dot) return false;
		else if(follow.size() != other.follow.size()) return false;
		return std::mismatch(follow.begin(), follow.end(), other.follow.begin()).first == follow.end();
	}
};

class lrstate : public std::vector<lrstateitem>
{
//	automachine::sheetrow rows;

public:
	bool operator == (const lrstate& other) const
	{
		return size() == other.size() &&
			(std::mismatch(begin(), end(), other.begin()).first == end());
	}
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
	AlgorithmArg(const tinygrammar& tig);
	kog::smart_vector<int> ists; // is terminate symbol
	eclosure::closure_array closures;
	firstset::vecintset firstsets;
	kog::smart_vector<const production*> plist;
	kog::smart_vector<int32> ntpi;
	std::list<lrstate> lrsts;
	std::list<kog::triple<const lrstate*, int32, nextstate> > sparsesheet;
	const tinygrammar* tig;

	void update_closure(lrstate& lrs) const;
	int32 get_next_symbol(const production& p, int32 idot) const;
	bool CheckLR1(const lrstate* cs) const;
	const lrstate* insert_new_state(const lrstate* A, int32 a, const lrstate* B);
	void get_firsts(int32 x, const sfollowset& f, sfollowset& nf) const;

	int32 get_lrsid(const lrstate* p) const;
	int32 get_pid(const production* p) const;
};

int32 AlgorithmArg::get_pid(const production* p) const
{
	return (int32)std::distance(tig->productions().begin(), 
						std::find_if(tig->productions().begin(), tig->productions().end(), 
							kog::composite_function(kog::get_ptr_t<production>(), 
								std::bind2nd(std::equal_to<const production*>(), p))));
}

int32 AlgorithmArg::get_lrsid(const lrstate* p) const
{
	return (int32)std::distance(lrsts.begin(), 
						std::find_if(lrsts.begin(), lrsts.end(), 
							kog::composite_function(kog::get_ptr_t<lrstate>(), 
								std::bind2nd(std::equal_to<const lrstate*>(), p))));
}


//std::ostream& operator<<(std::ostream& os, const lrstateitem& itm)
std::ostream& print_item(std::ostream& os, const lrstateitem& itm, const symholder_proxy& sholder)
{
	const production& p = *itm.prod;
	
	os<<sholder[p.left()].name<<" -> ";
	
	for(int32 j = 0; j < p.right_size(); ++ j)
	{
		if(itm.dot == j) os<<".";
		os<<sholder[p[j]].name<<" ";
	}
	if(itm.dot == p.right_size()) os<<".";
	os<<"\t,";
	for(sfollowset::const_iterator iter_f = itm.follow.begin(); iter_f != itm.follow.end(); ++ iter_f)
	{
		if (*iter_f < 0) os<<" #";
		else os<<" "<<sholder[*iter_f].name;
	}
	return os<<"\n";
}

void lranalyse::operator()(const tinygrammar& tig, lrmachine& mot)
{
	std::auto_ptr<AlgorithmArg> ptr_arg(new AlgorithmArg(tig));
	AlgorithmArg& arg = *ptr_arg;

	std::queue<const lrstate*> sQueue;
	sQueue.push(&(*kog::iterator_next(arg.lrsts.begin())));
	
	while(!sQueue.empty())
	{
		const lrstate* pRow = sQueue.front();
		sQueue.pop();

		typedef std::pair<int32, const lrstateitem*> etapair;
		typedef std::pair<int32, const production*> reducepair;
		std::vector<etapair> eats;// first: next symbol, second: ref lrstateitem
		std::vector<reducepair> reduces; // first: next symbol, second: reduce production
		for(lrstate::const_iterator iter = pRow->begin(); iter != pRow->end(); ++ iter)
		{
			const production& p = *iter->prod;
			if(iter->dot == p.right_size()) // reduce item
			{
				// using first(By) to make reduce item
				for(sfollowset::const_iterator iterf = iter->follow.begin(); iterf != iter->follow.end(); ++ iterf)
				{
					reduces.push_back(std::make_pair(*iterf, &p));
				}
			}
			else
			{
				eats.push_back(std::make_pair(p.right()[iter->dot], &(*iter)));
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
				item.prod = eats[j].second->prod;
				item.dot = eats[j].second->dot + 1;
				
				// update follow set
				item.follow = eats[j].second->follow;
				//int32 x = arg.get_next_symbol(*item.prod, item.dot);
				//arg.get_firsts(x, eats[j].second->follow, item.follow);

				s.push_back(item);
			}
			arg.update_closure(s);
			const lrstate* pNewS = arg.insert_new_state(pRow, eats[i].first, &s);
			if(pNewS != NULL)
				sQueue.push(pNewS);
		}

		// append reduce items
		for (size_t i = 0; i < reduces.size(); ++ i)
		{
#ifdef LALR1_SHIFT_FIRST
			if (std::find_if(eats.begin(), eats.end(), kog::composite_function(kog::mem_value(&etapair::first), 
				std::bind2nd(std::equal_to<int32>(), reduces[i].first))) != eats.end())
			{
				logwarning("reduce-shift conflict! drop reduce production[%d], when symbol (%s)", 
					arg.get_pid(reduces[i].second), tig.symbols()[reduces[i].first].name);
			}
#endif
			arg.sparsesheet.push_back(kog::make_triple(pRow, reduces[i].first, nextstate(reduces[i].second)));
#ifdef DEBUG_OUTPUT
			const char* name = reduces[i].first < 0 ? "#" : tig.symbols()[reduces[i].first].name;
			logstring("(I%d, %s) -> r%d", arg.get_lrsid(pRow), name, arg.get_pid(reduces[i].second));
#endif
		}

		// reduce - reduce conflict
		// reduce - shiftin conflict
		arg.CheckLR1(pRow);
	}

	make_machine(arg, tig, mot);

	// reset mout's morelist
	foreach (automachine::sheetrow& row, mot.sheet().begin(), mot.sheet().end())
	{
		row.more(NULL);
		row.code(-1);
	}
	mot.morelist().clear();
}

void lranalyse::make_machine(AlgorithmArg& arg, const tinygrammar& tig, lrmachine& mot) const
{
	lrmachine tmpm(tig);
	automachine::sparsesheet& sheet = tmpm.sheet();
	sheet.reset(arg.lrsts.size() + 1); // s0 is ending status
	tmpm.sstate() = 1;	// s1 is starting status
	
	typedef kog::triple<const lrstate*, int32, nextstate> gototriple;
	arg.sparsesheet.sort(kog::composite_function(kog::mem_value(&gototriple::first),
					kog::mem_value(&gototriple::first), std::less<const lrstate*>()));
	int32 nstate = (int32)arg.lrsts.size();
	sheet.reset(nstate);
	std::map<const lrstate*, int32> lrmap;
	int32 idx = 0;
	for (std::list<lrstate>::const_iterator iter = arg.lrsts.begin(); iter != arg.lrsts.end(); ++ iter) lrmap[&(*iter)] = idx ++;
	for (std::list<gototriple>::const_iterator iterI = arg.sparsesheet.begin(), iterJ; iterI != arg.sparsesheet.end(); )
	{
		int nc = 0;
		int32 A = lrmap[iterI->first];
		for (iterJ = iterI; iterJ != arg.sparsesheet.end() && iterJ->first == iterI->first; ++ iterJ) ++ nc;
		sheet[A].reset(nc);

		lrmachine::pinfoarray parray;
		parray.reset(iterI->first->size());
		nc = 0;
		for (lrstate::const_iterator iter_item = iterI->first->begin(); iter_item != iterI->first->end(); ++ iter_item)
		{
			parray[nc].dot = iter_item->dot;
			parray[nc].pid = (int32)std::distance<const production*>(tig.productions().get(), iter_item->prod);
			++ nc;
		}
		tmpm.morelist().push_back(parray);
		sheet[A].more(&tmpm.morelist().back());
		nc = 0;
		for (iterJ = iterI; iterJ != arg.sparsesheet.end() && iterJ->first == iterI->first; ++ iterJ, ++ nc)
		{
			switch(iterJ->third.sorp)
			{
			case 0: 
				sheet[A][nc] = automachine::gotoitem(iterJ->second, lrmap[iterJ->third.ps]);
				break;
			case 1:
				{
					//tinygrammar::vecprods::const_iterator iter_find = std::find
					int32 pid = (int32)std::distance(tig.productions().begin(), 
						std::find_if(tig.productions().begin(), tig.productions().end(), 
							kog::composite_function(kog::get_ptr_t<production>(), 
								std::bind2nd(std::equal_to<const production*>(), iterJ->third.pp))));
					sheet[A][nc] = automachine::gotoitem(iterJ->second, -(pid + 1));
				}
				break;
			default:
				fire("invalidate goto item!");
			}
			
		}
		iterI = iterJ;
	}

	for (automachine::sparsesheet::iterator iter = sheet.begin(); iter != sheet.end(); ++ iter)
	{
		std::sort(iter->begin(), iter->end(), 
			kog::composite_function(kog::mem_value(&automachine::gotoitem::first), kog::mem_value(&automachine::gotoitem::first),
				std::less<int32>()));
	}
	sheet.begin()->endings(1); // mark as ending status

	tmpm.swap(mot);
}

bool AlgorithmArg::CheckLR1(const lrstate* cs) const
{
	logstring("AlgorithmArg::CheckLR1 starting...");
	typedef std::list<kog::triple<const lrstate*, int32, nextstate> > lrrowlist;
	kog::smart_vector<int32> issused(tig->symbols().size() + 1); // last one is for ending symbol('#')
	memset(issused.get(), 0, issused.size_in_bytes());
	std::list<lrrowlist::const_reverse_iterator> needtoremove;
	for (lrrowlist::const_reverse_iterator  iter = sparsesheet.rbegin(); iter != sparsesheet.rend() && iter->first == cs; ++ iter)
	{
		int32 x = iter->second == -1 ? ((int32)issused.size() - 1) : iter->second;
		logstring("checking symbol '%s', sorp %d", iter->second == -1 ? "#" : tig->symbols()[x].name, iter->third.sorp);
		if(x < 0) fire("invalidate symbol!");
		else if(issused[x]) 
		{
#ifdef DEBUG_OUTPUT
			logstring("AlgorithmArg::CheckLR1, error lr state:\n");
			for (lrstate::const_iterator iter_item = cs->begin(); iter_item != cs->end(); ++ iter_item)
			{	
				print_item(kog::loggermanager::instance().get_logger().getos(), *iter_item, tig->symbols());
			}
#endif
			
#ifdef LALR1_SHIFT_FIRST
			if (issused[x] == 2 && iter->third.sorp) fire("not lr grammar~! reduce-reduce conflict");
#else
			fire("not lr grammar! reduce-reduce(shift) conflict");
#endif
		}
		else 
		{
			issused[x] = 1 + (iter->third.sorp != 0);
		}
	}
	return true;
}

void AlgorithmArg::update_closure(lrstate& li) const
{
	std::set<int32> cset;
	const prodholder_proxy& pholder = tig->productions();
	const symholder_proxy& sholder = tig->symbols();

	kog::smart_vector<sfollowset*> sfollows(sholder.size(), NULL);
	kog::smart_vector<int32> issupdated(sholder.size(), 0);
	for (lrstate::const_iterator iter = li.begin(); iter != li.end(); ++ iter)
	{
		const production& p = *iter->prod;
		if(iter->dot < p.right_size()) // not the end
		{
			int32 xx = p[iter->dot];
			if(!ists[xx]) 
			{
				for (eclosure::closure::const_iterator ie = closures[xx].begin(); ie != closures[xx].end(); ++ ie)
				{
					if (sfollows[*ie] == NULL) sfollows[*ie] = new sfollowset;
				}
				get_firsts(get_next_symbol(p, iter->dot + 1), 
					iter->follow, *sfollows[xx]);
				issupdated[xx] = 1;
			}
		}
	}

	while(true)
	{
		// find first need to update
		size_t i = 0;
		for (; i < sholder.size() && !issupdated[i]; ++ i);
		if(i == sholder.size()) break;
		issupdated[i] = 0;
		for (size_t j = ntpi[i]; j < plist.size() && plist[j]->left() == i; ++ j)
		{
			const production& p = *plist[j];
			if(!ists[p[0]])
			{
				int32 xn = (int32)sfollows[p[0]]->size();
				get_firsts(get_next_symbol(p, 1), *sfollows[i], *sfollows[p[0]]);
				issupdated[p[0]] |= sfollows[p[0]]->size() != xn; // need to update
			}
			else if(sholder[p[0]].name == NULL || *sholder[p[0]].name == '\0')
				fire("lranalyse, invalidate gramar: no eplison!");
		}
	}

	// set follows of closure's item
	lrstate tmps;
	for (lrstate::const_iterator iter = li.begin(); iter != li.end(); ++ iter)
	{
		const production& p = *iter->prod;
		if (sfollows[p.left()] == NULL || iter->dot != 0)
		{
			tmps.push_back(*iter);
		}
	}

	for (size_t i = 0; i < sfollows.size(); ++ i)
	{
		if(sfollows[i] == NULL) continue;
		for (size_t j = ntpi[i]; j < plist.size() && plist[j]->left() == i; ++ j)
		{
			lrstateitem itm;
			itm.prod = plist[j];
			itm.dot = 0;
			itm.follow = *sfollows[i];
			tmps.push_back(itm);
		}
		delete sfollows[i];
		sfollows[i] = NULL;
	}
	tmps.swap(li);
}

// A -> aB
const lrstate* AlgorithmArg::insert_new_state(const lrstate* A, int32 a, const lrstate* B)
{
	const lrstate* ps = NULL;
	const lrstate* rv = NULL;
	std::list<lrstate>::iterator iterfind = std::find(lrsts.begin(), lrsts.end(), *B);
	if(iterfind != lrsts.end())
	{
		// found an existing lrstate
		ps = &(*iterfind);
	}
	else
	{
		lrsts.push_back(*B);
		ps = &lrsts.back();
		rv = ps;
#ifdef DEBUG_OUTPUT
		static int32 ii = 2;
		logstring("I%d", ii++);
		for(size_t i = 0; i != ps->size(); ++ i)
		{
			const lrstateitem& itm = (*ps)[i];
			print_item(kog::loggermanager::instance().get_logger().getos(), itm, tig->symbols());
		}
		//logstring("\n");
#endif
	}
	sparsesheet.push_back(kog::make_triple(A, a, ps));
#ifdef DEBUG_OUTPUT
	const char* name = a < 0 ? "#" : tig->symbols()[a].name;
	logstring("(I%d, %s) -> I%d", get_lrsid(A), name, get_lrsid(ps));
#endif
	return rv;
}

void AlgorithmArg::get_firsts(int32 x, const sfollowset& f, sfollowset& nf) const
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
	return p.right()[idot];
}

AlgorithmArg::AlgorithmArg(const tinygrammar& gin)
: ists(gin.symbols().size() + 1, 0)
, plist(gin.productions().size(), NULL)
, ntpi(gin.symbols().size(), -1)
, tig(&gin)
{
	const symholder_proxy& sholder = tig->symbols();
	const prodholder_proxy& pholder = tig->productions();
	const size_t M = pholder.size();
	const size_t N = sholder.size();

	std::transform(pholder.begin(), pholder.end(), plist.begin(), kog::get_ptr_t<production>());
	// sort by production left
	kog::depointer_t<const production> dpp;
	std::sort(plist.begin(), plist.end(), kog::composite_function(dpp, dpp, pleft_less()));
	for(size_t i = 0; i < plist.size(); ++ i)
		if(ntpi[plist[i]->left()] == -1) ntpi[plist[i]->left()] = (int32)i;

	// get e-closure of every non-terminate symbol
	eclosure ec_algorithm(*tig, closures);
	ec_algorithm.invoke();

	// get first set of every non-terminate symbol 
	firstset fs_algorithm(gin, firstsets);
	fs_algorithm.invoke();

	std::transform(sholder.begin(), sholder.end(), ists.begin(),
		kog::mem_value(&symbol::ist));
	// Symbol[N] = S'; S' -> S

	// update 
	lrsts.resize(2);
	// 0: ending status
	// 1: start status
	{
		lrstate& start = *kog::iterator_next(lrsts.begin());
		int32 start_symbol = tig->starts();
		if(start_symbol < 0 || start_symbol >= (int32)N)
			fire("invalidate start symbol index(%d)", start_symbol);
//		size_t nc = std::distance(plist.begin() + npti[start_symbol], 
//			std::find_if(plist.begin() + npti[start_symbol], plist.end(), 
//			kog::composite_function(kog::composite_function(kog::depointer_t<const production*>(), kog::member_value(&production::left)))))
		int32 nc = 0;
		for (size_t i = ntpi[start_symbol]; i < plist.size() && plist[i]->left() == start_symbol; ++ i) ++ nc;
		start.resize(nc);
		for (int32 i = ntpi[start_symbol], j = 0; j < nc; ++ i, ++ j)
		{
			lrstateitem& itm = start[j];
			itm.prod = plist[i];
			itm.dot = 0;
			itm.follow.insert(-1); // -1 is ending symbol
		}

		update_closure(start);

#ifdef DEBUG_OUTPUT
		logstring("\nI1\n");
		for(size_t i = 0; i != start.size(); ++ i)
		{
			const lrstateitem& itm = start[i];
			print_item(kog::loggermanager::instance().get_logger().getos(), itm, tig->symbols());
		}
		logstring("\n");
#endif
	}
	
	// add new goto row
	const lrstate* s1 = &(*kog::iterator_next(lrsts.begin()));
	const lrstate* s0 = &(*lrsts.begin());
	sparsesheet.push_back(kog::make_triple(s1, tig->starts(), nextstate(s0)));
}
