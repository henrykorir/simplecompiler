#include "nfa2dfa.h"

#include <list>
#include <queue>
#include <vector>
#include <functional>
#include <algorithm>
#include <iterator>
#include <stringX.h>
#include <functionalX.h>
#include <scerror.h>

#include <iostream>

#ifdef DEBUG_OUTPUT
#include <logger.h>
#include "../test/gio.h"
#endif

using namespace compile;
using namespace compile::ga;

void nfa2dfa::make_sure_rg(const tinygrammar& tig)
{
	const tinygrammar::vecprods& prods = tig.productions();
	const symholder sholder = tig.symbols();

	for(size_t i = 0; i < prods.size(); ++ i)
	{
		const production& p = prods[i];
		if(p.right_size() != 1 && p.right_size() != 2)
			fire("not a left grammar!");
		else if(p.right_size() == 2 && !(sholder[p[0]].ist && !sholder[p[1]].ist))
			fire("not a left grammar!");
	}
}

void nfa2dfa::operator()(const tinygrammar& minput, tinygrammar& motput)
{
	make_sure_rg(minput);
	tinygrammar tg1, tg2;
	todfa(minput, tg1);
#ifdef DEBUG_OUTPUT
	logstring("\n[nfa2dfa] \n");
	gwriter gw(kog::loggermanager::instance().get_logger().getos());
	gw<<tg1;
#endif
	mini_status(tg1, tg2);
//	mini_status(minput.gettinyg(), endings, tg2);

	tg2.swap(motput);
}

void nfa2dfa::todfa(const tinygrammar& tig, tinygrammar& tog)
{
	/// get e-closure
	const tinygrammar::vecprods& prods = tig.productions();
	const symholder& sholder = tig.symbols();

	kog::smart_vector<int32> ist(sholder.size());
	const int32 virtual_ending = sholder.size();
	kog::smart_vector<std::set<int32> > eClosures(sholder.size() + 1);
	for (size_t i = 0; i < sholder.size(); ++ i) 
		if(!(ist[i] = sholder[i].ist)) eClosures[i].insert(i);
	kog::smart_vector<const production*> plist;
	plist.reset(prods.size());
	for(size_t i = 0; i < prods.size(); ++ i) plist[i] = &prods[i];
	// sort by production left
	kog::depointer_t<const production> dpp;
	std::sort(plist.begin(), plist.end(), kog::composite_function(dpp, dpp, pleft_less()));
	kog::smart_vector<int32> ntpi(sholder.size(), -1);
	for(size_t i = 0; i < plist.size(); ++ i)
		if(ntpi[plist[i]->left()] == -1) ntpi[plist[i]->left()] = i;
	// e-closure
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
	}

	std::deque<symbol> slist;
	kog::smart_vector<int32> sidmap(sholder.size(), -1);
	for(size_t i = 0, j = 0; i < sholder.size(); ++ i)
	{
		if(sholder[i].ist && i != tig.eplisons()) // the output grammar is not contain eplison
		{
			sidmap[i] = j ++;
			slist.push_back(sholder[i]);
			slist.back().sid = sidmap[i];
		}
	}
	const char ending_sym_buffer[2] = {-1, '\0'};
	const int32 endings = tig.endings() != -1 ? sidmap[tig.endings()] : (int32)slist.size();
	if(tig.endings() == -1)
	{
	// we create an new ending symbol here, name is -1(EOF), which is no printable
		symbol asym;
		memset(&asym, 0, sizeof(asym));
		asym.name = ending_sym_buffer;
		asym.Lname = 1;
		asym.ist = 1;
		asym.sid = endings;
		slist.push_back(asym);
	}

	const int32 first_nontsid = (int32)slist.size();
	std::list<std::set<int32> > newS;
	std::list<production> newP;
	std::queue<std::pair<int32, std::set<int32>*> > squeue;
	newS.push_back(eClosures[tig.starts()]);
	squeue.push(std::make_pair(first_nontsid, &newS.back()));
	while(!squeue.empty())
	{
		std::set<int32>* cs = squeue.front().second;
		int32 newL = squeue.front().first;
		squeue.pop();
		std::vector<std::pair<int32, int32> > t;
		for(std::set<int32>::const_iterator iter = cs->begin(); iter != cs->end(); ++ iter)
		{
			int32 x = ntpi[*iter];
			if (x == -1) fire("can't be -1");
			for (size_t v = x; v < plist.size() && plist[v]->left() == *iter; ++ v)
			{
				int32 r0 = plist[v]->right().at(0);
				if(ist[r0]/* && r0 != tig.eplisons()*/)
				{
					if(plist[v]->right_size() == 1) // plist[v]->left() is an ending symbol
					{
						if(r0 != tig.eplisons() && r0 != tig.endings()) t.push_back(std::make_pair(r0, virtual_ending));
					}
					else if(plist[v]->right_size() == 2 && r0 != tig.eplisons())
					{
						t.push_back(std::make_pair(r0, plist[v]->right().at(1)));
					}
				}
			}
		}
		std::sort(t.begin(), t.end(), intpair_less());
		for(size_t i = 0; i < t.size();)
		{
			std::set<int32> s;
			int32 xx = t[i].first;
			size_t j = i;
			while(j < t.size() && t[j].first == xx)
			{
				s.insert(t[j].second);
				++ j;
			}
			i = j;
			// [i, j)
			// get e-closure of the new state
			while(true)
			{
				size_t osize = s.size();
				std::set<int32> tmps(s);
				for(std::set<int32>::const_iterator iter = s.begin(); iter != s.end(); ++ iter)
				{
					tmps.insert(eClosures[*iter].begin(), eClosures[*iter].end());
				}
				tmps.swap(s);
				if(s.find(0) != s.end()) 
				{
					s.erase(s.find(0));
				}
				if(osize == s.size()) break;
			}
			// so we can try to create a new state or find an exist state
			int32 fidx = (int32)std::distance(newS.begin(), std::find(newS.begin(), newS.end(), s));
			if(fidx == newS.size()) 
			{
			//	std::set<int32>::iterator iters = s.find(-1);
			//	if(iters != s.end()) s.erase(iters);
				newS.push_back(s);
				squeue.push(std::make_pair(fidx + first_nontsid, &newS.back()));
			}
			// make a new production
			int R[2] = {sidmap[xx], fidx + first_nontsid};
			newP.push_back(production(newL, R, 2));
			if(s.find(virtual_ending) != s.end()) // s is an ending status
			{
				newP.push_back(production(fidx + first_nontsid, &endings, 1));
			}
		}
	}

	std::list<std::string> tmpSlist;
	for(size_t i = 0; i < newS.size(); ++ i)
	{
		symbol asym;
		memset(&asym, 0, sizeof(asym));
		tmpSlist.push_back(stringX::format("S%d", tmpSlist.size()));
		asym.name = tmpSlist.back().c_str();
		asym.Lname = (int32)tmpSlist.back().size();
		slist.push_back(asym);
	}

	tinygrammar mac(slist.begin(), slist.end(), newP.begin(), newP.end(), first_nontsid);
	mac.endings() = endings;
	mac.swap(tog);
}

void nfa2dfa::mini_status(const tinygrammar& tig, tinygrammar& tog)
{
	/// mini status, input grammar must be a dfa
	//  	because tig is dfa, so A -> a, A is an ending symbol
	typedef tinygrammar::vecprods prodholder;
	const symholder& sholder = tig.symbols();
	const prodholder& pholder = tig.productions();

	// init: two parition, {1} non-ending symbol, {0} ending-symbol
	std::deque<std::set<int32> > pi(2);
	kog::smart_vector<int32> smap(sholder.size(), -1); // the last one is virtual ending
	std::vector<const production*> plist;
//	std::transform(pholder.begin(), pholder.end(), plist.begin(), kog::get_ptr_t<production>());
	// ending and non-ending set
	if(tig.endings() == -1) fire("[nfa2dfa::minis] ending symbol can't be -1!");
	std::set<int32> endings; // store ending symbols' sid
	for(size_t i = 0; i < pholder.size(); ++ i)
	{
		const production& p = pholder[i];
		if(p.right_size() == 1 && p.right()[0] == tig.endings())
		{
			pi[0].insert(p.left());
			endings.insert(p.left());
			smap[p.left()] = 0;
		}
		else plist.push_back(&p);
	}
	for(size_t i = 0; i < sholder.size(); ++ i)
	{
		if(!sholder[i].ist && smap[i] == -1)
		{
			smap[i] = 1;
			pi[1].insert(i);
		}
	}
	if(pi[1].empty() || pi[0].empty())
	{
		tinygrammar tmpg(tig);
		tog.swap(tmpg);
		return ;
	}

	kog::depointer_t<const production> dpp;
	std::sort(plist.begin(), plist.end(), kog::composite_function(dpp, dpp, pleft_less()));
	kog::smart_vector<int32> ntpi(sholder.size(), -1);
	for(size_t i = 0; i < plist.size(); ++ i)
		if(ntpi[plist[i]->left()] == -1) ntpi[plist[i]->left()] = i;
	size_t opisize = 0;
	do{
		opisize = pi.size();
		for(size_t i = 0; i < opisize; ++ i)
		{
			const std::set<int32>& I = pi[i];
			std::vector<const production*> t;
			for(std::set<int32>::const_iterator iter = I.begin(); iter != I.end(); ++ iter)
			{
				for(size_t x = ntpi[*iter]; x < plist.size() && plist[x]->left() == *iter; ++ x)
				{
					t.push_back(plist[x]);
				}
			}
			std::sort(t.begin(), t.end(), kog::composite_function(dpp, dpp, pfirstright_less()));

			// if can split or not
			split(t, smap, pi);
		}
#ifdef DEBUG_OUTPUT
		{
			logstring("pi = { ");
			const char* sep = "";
			std::ostream& os = kog::loggermanager::instance().get_logger().getos();
			for(std::deque<std::set<int32> >::const_iterator iter = pi.begin(); iter != pi.end(); ++ iter, sep = ", ")
			{
				std::copy(iter->begin(), iter->end(), std::ostream_iterator<int32>(os<<sep<<"{ ", " "));
				os<<"}";
			}
			os<<" }"<<std::endl;
		}
#endif
	}while(opisize != pi.size());

	// create new grammar
	std::list<production> newp;
	std::list<symbol> news;
	// first we count how many terminate symbol
	const int32 first_nontsid = std::count_if(sholder.begin(), sholder.end(),
			kog::composite_function(kog::mem_value(&symbol::ist), std::bind2nd(std::not_equal_to<int8>(), int8(0))))
		- (tig.eplisons() >= 0 && tig.eplisons() < sholder.size() ? 1 : 0); // don't count eplison
	for(size_t i = 0, j = 0; i < sholder.size(); ++ i)
	{
		if(sholder[i].ist && i != tig.eplisons()) // we also don't use eplison here too
		{
			smap[i] = j ++;
			news.push_back(sholder[i]);
			news.back().sid = smap[i];
		}
		else if(!sholder[i].ist)
			smap[i] += first_nontsid;
	}
//	const int32 first_nontsid = news.size();
	std::list<std::string> strbuf;
	for(size_t i = 0; i < pi.size(); ++ i)
	{
		strbuf.push_back(stringX::format("S%d", i));
		symbol asym;
		memset(&asym, 0, sizeof(asym));
		asym.name = strbuf.back().c_str();
		asym.Lname = (int32)strbuf.back().size();
		asym.sid = i + first_nontsid;
		news.push_back(asym);
		
		// insert ending productions A -> EOF
		if(endings.find(*pi[i].begin()) != endings.end())
		{
			newp.push_back(production(asym.sid, &smap[tig.endings()], 1));
		}
	}

	for(size_t i = 0; i < pholder.size(); ++ i)
	{
		const production& p = pholder[i];
		int R[2] = {smap[p[0]], 0};
		if(R[0] == -1) continue; // not used production
		int L = smap[p.left()];
		if(p.right_size() == 2)
		{
			R[1] = smap[p[1]];
			production ptmp(L, R, 2);
			newp.push_back(ptmp);
		}
		else 
		{
			production ptmp(L, R, 1);
			newp.push_back(ptmp);
		}
	}

	//for (std::list<production>::const_iterator iter = newp.begin(); iter != newp.end(); ++ iter)
	//{
	//	std::cout<<iter->left()<<" -> ";
	//	std::copy(iter->right().begin(), iter->right().end(),
	//		std::ostream_iterator<int32>(std::cout, "\t"));
	//	std::cout<<std::endl;
	//}
	newp.sort(prod_less());
	newp.unique(p_equal());

	tinygrammar tmpg(news.begin(), news.end(), newp.begin(), newp.end(), smap[tig.starts()], -1, smap[tig.endings()]);
	tog.swap(tmpg);
}

inline int32 get_r1(const production* p, int32 virtual_ending)
{
	if(p->right_size() == 1)
		return virtual_ending;
	return p->right()[1];
}

void nfa2dfa::split(const std::vector<const production*>& t, kog::smart_vector<int32>& smap, std::deque<std::set<int32> >& s) const
{
	const int32 virtual_ending = (int32)smap.size() - 1;
	for(size_t i = 0, j = 1; i < t.size(); i = j)
	{
		std::set<int32> ts;
		const production* p = t[i];
		int32 newSid = smap[t[i]->left()];
		std::set<int32> caneats;
		// [i, j)
		for(j = i; j < t.size() && t[j]->right()[0] == p->right()[0]; ++ j)
		{
			caneats.insert(t[j]->left());
			ts.insert(smap[get_r1(t[j], virtual_ending)]);
		}
		if(ts.size() <= 1 && caneats.size() == s[newSid].size()) continue;
		int32 x = (int32)s.size();
		int32 ns = 0;
		if(caneats.size() != s[newSid].size())
		{
			// we leave uneat sets
			ns = ts.size();
			s.resize(x + ns);
			//s[newSid] -= caneats;
			std::deque<int32> vtmp;
			std::set_difference(s[newSid].begin(), s[newSid].end(), caneats.begin(), caneats.end(), std::back_inserter(vtmp));
			if(vtmp.empty())
			{
#ifdef DEBUG_OUTPUT
				std::ostream& os = kog::loggermanager::instance().get_logger().getos();
				std::copy(s[newSid].begin(), s[newSid].end(), std::ostream_iterator<int32>(os<<"\n", " "));
				std::copy(caneats.begin(), caneats.end(), std::ostream_iterator<int32>(os<<"\n", " "));
				std::cout<<"\n";
#endif
				fire("empty!");
			}
			s[newSid].clear();
			s[newSid].insert(vtmp.begin(), vtmp.end());
			for(std::set<int32>::const_iterator iter = ts.begin(); iter != ts.end(); ++ iter)
			{
				for(size_t k = i; k < j; ++ k)
				{
					if(smap[get_r1(t[k], virtual_ending)] == *iter)
					{
						s[x].insert(t[k]->left());
					}
				}
				++ x;
			}
		}
		else
		{
			ns = ts.size() - 1;
			s.resize(x + ns);
			//bool is_ending = s[newSid].find(virtual_ending) != s[newSid].end();
			s[newSid].clear();
			for(size_t k = i; k < j; ++ k) // rnewSiize() - 1eset s[smap[ts[0]]]
			{
				if(smap[get_r1(t[k], virtual_ending)] == newSid) s[newSid].insert(t[k]->left());
			}
			
			for(std::set<int32>::const_iterator iter = ts.begin(); iter != ts.end(); ++ iter)
			{
				if(*iter == newSid) continue;
				for(size_t k = i; k < j; ++ k)
				{
					if(smap[get_r1(t[k], virtual_ending)] == *iter)
					{
						s[x].insert(t[k]->left());
					}
				}
				++ x;
			}
		}

		// reset smap
		for(size_t m = x - ns; m < s.size(); ++ m)
		{
			for(std::set<int32>::const_iterator iter = s[m].begin(); iter != s[m].end(); ++ iter)
				smap[*iter] = (int32)m;
		}
		return;
	}
}
