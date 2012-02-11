#include "dfa2machine.h"
#include <deque>
#include <scerror.h>
#include <stringX.h>

using namespace compile;
using namespace compile::ga;

void dfa2machine::operator()(const tinygrammar& tig, automachine& mot)
{
	// make sure tig is dfa grammar
	make_sure_dfa(tig);

	const tinygrammar::vecprods& prods = tig.productions();
	const symholder& sholder = tig.symbols();

//	const int32 scount = std::count_if(sholder.begin(), sholder.end(), 
//			kog::composite_function(kog::mem_value(&symbol::sid), 
//				std::bind2nd(std::equal_to<int8>(), int8(0))));
	kog::smart_vector<int32> ms(sholder.size(), -1);
	int32 scount = 0;
	for(size_t i = 0; i < sholder.size(); ++ i) // j=0: the start state, j=1: the end state
	{
		if(!sholder[i].ist) ms[i] = scount ++;
	}
	kog::smart_vector<std::deque<automachine::gotoitem> > rows(scount);
	kog::smart_vector<int32> rowinfo(scount);
	memset(rowinfo.get(), 0, rowinfo.size_in_bytes());
	for(size_t i = 0; i < prods.size(); ++ i)
	{
	// create goto sheet
		const production& p = prods[i];
		std::deque<automachine::gotoitem>& r = rows.at(ms[p.left()]); 
		int32& ri = rowinfo.at(ms[p.left()]);
		switch(p.right_size())
		{
		case 1: ri = 1; break; // mark as ending status
		case 2: 
			{
				bool bfound = false;
				for(size_t j = 0; j < r.size(); ++ j)
				{
					if(r[j].first == p[0])
						if(r[j].second != ms[p[1]]) fire("dfa2m, A->aB, A->aC error");
						else {bfound = true;  break;}
				}
				if(bfound) continue;
				r.push_back(automachine::gotoitem(p[0], ms[p[1]]));
			}
		}
	}

	// create new sheet 
	automachine::shared_sheet sheet(new automachine::sparsesheet(scount));
	for(int32 i = 0; i < scount; ++ i)
	{
		automachine::sheetrow& r = sheet->at(i);
		r.endings(rowinfo[i]);
		r.reset(rows[i].size());
		std::copy(rows[i].begin(), rows[i].end(), r.begin());
	}
	automachine mtmp(sheet); // tmp machine
	mtmp.sstate() = ms[tig.starts()];
	mot.swap(mtmp);
	mot.init();
}

void dfa2machine::make_sure_dfa(const tinygrammar& tig) const
{
// make sure the input grammar is dfa and eplison is not in the grammar, and there is no single production
	if(tig.endings() == -1) fire("dfa2m, input grammar's ending symbol can't be -1");
	if(tig.eplisons() != -1) fire("dfa2m, input grammar can't contain eplison!");
	const tinygrammar::vecprods& pholder = tig.productions();
	const symholder& sholder = tig.symbols();
	for(size_t i = 0; i < pholder.size(); ++ i)
	{
		const production& p = pholder[i];
		if(p.right_size() > 2) fire("dfa2m, production right <= 2");
		else if(p.right_size() <= 0) fire("dfa2m, production right > 0");
		else if(p.right_size() == 1 && p[0] != tig.endings()) fire("dfa2m, A->B is wrong");
		else if(p.right_size() == 2 && !(sholder[p[0]].ist && !sholder[p[1]].ist))
			fire("dfa2m, A->aB is allow!");
	}
}
