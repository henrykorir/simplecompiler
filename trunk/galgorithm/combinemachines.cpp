#include "combinemachines.h"
#include <scerror.h>
#include <stringX.h>
#include <queue>
#include <logger.h>

using namespace compile;
using namespace compile::ga;

int32 combine_machines::find_or_insert(const closure& c)
{
	closure_index_map::const_iterator iter = cmap_.find(&c);
	if (iter != cmap_.end()) return iter->second;
	closures_.push_back(c);
	int32 cid = (int32)cmap_.size();
	cmap_[&closures_.back()] = cid;
	rows_.resize(rows_.size() + 1);
	automachine::sheetrow& r = rows_.back();
	
	// check ending status, and symbol index
	foreach (const citem& x, c.begin(), c.end())
	{
		if (input_macs_[x.first]->sheet().at(x.second).endings())
		{
			if (r.endings()) fire("string can eat by two different machines %d and %d", r.code(), x.first);
			r.endings(1);
			r.code(x.first);
		}
	}

	std::ostream& os = kog::loggermanager::instance().get_logger().getos();
	os<<stringX::format("closure[%d] = { ", cid);;
	foreach (const citem& x, c.begin(), c.end())
	{
		os<<"("<<x.first<<", "<<x.second<<") ";
	}
	os<<"}"<<std::endl;

	return cid;
}

void combine_machines::update_sheet_row(int32 rid)
{
	const closure& cr = closures_.at(rid);

	std::map<int32, closure> firstmetas;

	// find all first metas, and next closures
	foreach (const citem& ct, cr.begin(), cr.end())
	{
		const automachine::sheetrow& row = input_macs_[ct.first]->sheet().at(ct.second);
		foreach (const automachine::gotoitem& gt, row.begin(), row.end())
		{
			firstmetas[gt.first].push_back(citem(ct.first, gt.second));
#if (defined _WIN32 || defined WIN32)
			if (std::isprint(gt.first, std::locale("")))
#else
			if (std::isprint(gt.first))
#endif
				logstring("closure[%d] -> '%c' (%d,%d)", rid, (char)gt.first, ct.first, gt.second);
			else logstring("closure[%d] -> %d (%d,%d)", rid, gt.first, ct.first, gt.second);
		}
	}
	// sort 
	typedef std::pair<const int32, closure> meta_closure_pair;
	foreach (meta_closure_pair& c, firstmetas.begin(), firstmetas.end())
	{
		std::sort(c.second.begin(), c.second.end(), 
			kog::pair_less<citem::first_type, citem::second_type>());
		c.second.erase(std::unique(c.second.begin(), c.second.end(), 
			kog::pair_equal<citem::first_type, citem::second_type>()), c.second.end());
	}

	std::ostream& os = kog::loggermanager::instance().get_logger().getos();
	os<<"firstmetas = { ";
	foreach (meta_closure_pair& c, firstmetas.begin(), firstmetas.end())
	{
#if (defined _WIN32 || defined WIN32)
			if (std::isprint(c.first, std::locale("")))
#else
			if (std::isprint(c.first))
#endif
			os<<"'"<<(char)c.first<<"' ";
		else os<<c.first<<" ";
	}
	os<<"}"<<std::endl;
	
	// update closures and newsheet
	automachine::sheetrow& start = rows_.at(rid);
	start.reset(firstmetas.size());
	automachine::sheetrow::value_type* gitm = start.get();
	
	foreach (const meta_closure_pair& c, firstmetas.begin(), firstmetas.end())
	{
		// sort closure		
		gitm->first = c.first;
		gitm->second = find_or_insert(c.second);
		++ gitm;
	}
	std::sort(start.begin(), start.end(), intpair_less());
}

void combine_machines::operator()(const machine_vector& inputmacs, automachine& otput_mac)
{
	logstring("start combine_machines, input %d machines", inputmacs.size());
	// check argument is validate?
	if (std::find(inputmacs.begin(), inputmacs.end(), &otput_mac) != inputmacs.end())
		fire("output machine is in input machine list!");
	foreach (const automachine* m, inputmacs.begin(), inputmacs.end())
		if (m->sheet().size() == 0) fire("invalidate input machine");

	// clean all tmp content
	closures_.clear();
	rows_.clear();
	cmap_.clear();
	// set first row's sheet and status
	closure firstc;
	closures_.resize(1);
	for (size_t i = 0; i < inputmacs.size(); ++ i)
	//foreach (const automachine* m, inputmacs.begin(), inputmacs.end())
	{
		closures_.back().push_back(citem(i, inputmacs[i]->sstate()));
	}
	rows_.resize(1);
	std::sort(rows_[0].begin(), rows_[0].end(), intpair_less());
	cmap_[&closures_.back()] = 0;

	// update all rows
	size_t rid = 0;
	while (rid < rows_.size())
	{
		// start to expand newsheet[rid]
		update_sheet_row(rid ++);
	}

	// copy temporary sheet rows to output machine
	automachine::shared_sheet new_sheet(new automachine::sparsesheet(rows_.begin(), rows_.end()));
	
	automachine tmpmac(new_sheet);
	tmpmac.sstate() = 0;
	tmpmac.init();
	tmpmac.swap(otput_mac);

	// debug output
	logstring("combine machine result");
	logstring("start is %d", otput_mac.sstate());
	std::ostream& os = kog::loggermanager::instance().get_logger().getos();
	size_t ir = 0;
	foreach (const automachine::sheetrow& r, otput_mac.sheet().begin(), otput_mac.sheet().end())
	{
		os<<stringX::format("row[%d] %s code(%d) \n\t", 
			ir++, r.endings() ? "ending" : "", r.code());
		foreach (const automachine::gotoitem& it, r.begin(), r.end())
		{
#if (defined _WIN32 || defined WIN32)
			if (std::isprint(it.first, std::locale("")))
#else
			if (std::isprint(it.first))
#endif
				os<<stringX::format("('%c',%d) ", (char)it.first, it.second);
			else os<<stringX::format("(%d,%d) ", it.first, it.second);
		}
		os<<std::endl;
	}
}

bool combine_machines::closure_less::operator ()(const compile::ga::combine_machines::closure &c1, const compile::ga::combine_machines::closure &c2) const
{
	if (c1.size() != c2.size()) return c1.size() < c2.size();
	size_t n = c1.size();
	for (size_t i = 0; i < n; ++ i)
	{
		if (c1[i].first < c2[i].first) return true;
		else if(c1[i].first == c2[i].first && c1[i].second < c2[i].second) return true;
	}
	return false;
}

bool combine_machines::closure_equal::operator()(const closure& c1, const closure& c2) const
{
	if (c1.size() != c2.size()) return false;
	size_t n = c1.size();
	for (size_t i = 0; i < n; ++ i)
	{
		if (c1[i] != c2[i]) return false;
	}
	return true;
}

int32 combine_machines::closure_indexer::operator()(const closure* c) const
{
	int32 x = 0;
	foreach (const citem& it, c->begin(), c->end())
	{
		x += it.first * 100 + it.second;
	}
	return x;
}
