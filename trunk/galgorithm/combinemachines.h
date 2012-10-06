// combinemachines.h : combine given machines to a machine
//

#ifndef _COMBINE_MACHINES_H_SC_
#define _COMBINE_MACHINES_H_SC_

#include <macros.h>
#include "grammaralgorithm.h"
#include <arrayX.h>
#include <automachine.h>
#include <map>
#include <deque>
#include <functionalX.h>
#include <buckethash.h>

NAMESPACE_BEGIN(compile);
NAMESPACE_BEGIN(ga);

class combine_machines : public grammar_algorithm
{
private:
	typedef kog::smart_vector<const automachine*> machine_vector;
public:
	combine_machines(const automachine* const input_macs[], size_t N, automachine& otput_mac)
		: grammar_algorithm("combine automachines to one machine")
		, input_macs_(input_macs, input_macs + N)
		, mout_(&otput_mac)
		, cmap_(1000)
	{
		//input_macs_.reset(input_macs_.size());
		//for (size_t i = 0; i < input_macs_.size(); ++ i)
		//{
		//	input_macs_[i].first = input_macs[i];
		//	input_macs_[i].second = sids[i];
		//}
		//std::sort(input_macs_.begin(), input_macs_.end());
	}
public:
	/* overwrite */ virtual void invoke()
	{
		(*this)(input_macs_, *mout_);
	}
private:
	// input machines
	machine_vector input_macs_;
	// output machine
	automachine* mout_;
// core proccess
private:
	// core call
	void operator()(const machine_vector& inputmacs, automachine& otput_mac);
private:
	typedef std::pair<int32, int32> citem; // first: sheet, second: status of the sheet
	typedef std::deque<citem> closure;
private:
	struct closure_less : public std::binary_function<closure, closure, bool>
	{
		bool operator()(const closure& c1, const closure& c2) const;
	};

	struct closure_indexer : public std::unary_function<const closure*, int32>
	{
		int32 operator()(const closure* c) const;
	};

	struct closure_equal : public std::binary_function<closure, closure, bool>
	{
		bool operator()(const closure& c1, const closure& c2) const;
	};

	typedef kog::pointer_less<closure, closure_less> pclosure_less;
	typedef kog::pointer_equal<closure, closure_equal> pclosure_equal;
	//typedef std::map<const closure*, int32, pclosure_less> closure_index_map;
	typedef kog::buckethash<const closure*, int32, closure_indexer, pclosure_equal> closure_index_map;
	typedef std::deque<closure> deqclosures;
	typedef std::deque<automachine::sheetrow> deqsheetrow;
private:
	void update_sheet_row(int32 rid);
	int32 find_or_insert(const closure& c);
private:
	closure_index_map cmap_;
	deqclosures closures_;
	deqsheetrow rows_;
};

NAMESPACE_END(ga);
NAMESPACE_END(compile);

#endif