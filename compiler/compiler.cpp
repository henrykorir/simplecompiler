#include "compiler.h"
#include <functional>
#include <algorithm>
using namespace sc;
using namespace compile;

state_machine compiler::get_number_machine()
{
	return instance().machines["number"];
}

state_machine compiler::get_symbol_machine()
{
	return instance().machines["symbol"];
}

state_machine compiler::get_string_machine()
{
	return instance().machines["string"];
}
bool compiler::is_separator(int32 elem)
{
	return std::find(instance().separators.begin(), 
			instance().separators.end(), elem) != instance().separators.end();
}

int compiler::get_all_machines(std::list<state_machine*>& mlist)
{
	for(std::map<std::string, state_machine>::iterator iter = instance().machines.begin(); iter != instance().machines.end(); ++ iter)
		mlist.push_back(&iter->second);
	return (int32)mlist.size();
}

void compiler::initmachines()
{
	{
	//kog::share_ptr<state_machine::sparsesheet> numsheet(new state_machine::sparsesheet());
	state_machine::shared_sheet numsheet(new state_machine::sparsesheet());
	// init number machine, current not support 1.4E3
	state_machine::sparsesheet& ns = *numsheet;
	numsheet->reset(5);
	ns[0].reset(11); ns[0].endings(0);
	ns[1].reset(11); ns[1].endings(1);
	ns[2].reset(10); ns[2].endings(0);
	ns[3].reset(10); ns[3].endings(1);
	ns[4].endings(1);
	for(int meta = '0', j = 0; meta <= '9'; ++ meta, ++ j)
	{
		ns.at(0)[j] = std::make_pair(meta, 1);
		ns.at(1)[j] = std::make_pair(meta, 1);
		ns.at(2)[j] = std::make_pair(meta, 3);
		ns.at(3)[j] = std::make_pair(meta, 3);
	}
	ns.at(0)[10] = std::make_pair('.', 2);
	ns.at(1)[10] = std::make_pair('.', 2);
	std::sort(ns[0].get(), ns[0].get() + ns[0].size(), intpair_less());
	std::sort(ns[1].get(), ns[1].get() + ns[1].size(), intpair_less());
	std::sort(ns[2].get(), ns[2].get() + ns[2].size(), intpair_less());
	std::sort(ns[3].get(), ns[3].get() + ns[3].size(), intpair_less());
	instance().machines["number"] = state_machine(numsheet);
	}

	// init symbol machine
	{
	//kog::share_ptr<state_machine::sparsesheet> symsheet(new state_machine::sparsesheet());
	state_machine::shared_sheet symsheet(new state_machine::sparsesheet());
	state_machine::sparsesheet& ss = *symsheet;
	symsheet->reset(2);
	ss[0].reset(53); ss[0].endings(0);
	ss[1].reset(63); ss[1].endings(1);
	for(int meta = 'A', j = 0; meta <= 'Z'; ++ meta)
	{
		ss.at(0)[j] = std::make_pair(meta, 1);
		ss.at(1)[j] = std::make_pair(meta, 1);
		ss.at(0)[++j] = std::make_pair(meta - 'A' + 'a', 1);
		ss.at(1)[j++] = std::make_pair(meta - 'A' + 'a', 1);
	}
	for(int meta = '0', j = 52; meta <= '9'; ++ meta, ++ j)
	{
		ss.at(1)[j] = std::make_pair(meta, 1);
	}
	ss.at(0)[52] = std::make_pair('_', 1);
	ss.at(1)[62] = std::make_pair('_', 1);
	std::sort(ss[0].get(), ss[0].get() + ss[0].size(), intpair_less());
	std::sort(ss[1].get(), ss[1].get() + ss[1].size(), intpair_less());
	state_machine tmpmac(symsheet);
	instance().machines["symbol"] = tmpmac;
	}
	
	{
		// init string machine
	state_machine::shared_sheet strsheet(new state_machine::sparsesheet());
	state_machine::sparsesheet& ss = *strsheet;
	const std::string tmp = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
	instance().printablechars.reset(tmp.size());
	for(size_t i = 0; i < tmp.size(); ++ i) instance().printablechars[i] = tmp[i];
	strsheet->reset(4);
	ss[0].reset(1);   ss[0].endings(0);
	ss[1].reset(128); ss[1].endings(0);
	ss[2].reset(128); ss[2].endings(0); ss[3].endings(1);
	//ss[3].reset(0);
	ss[0][0] = std::make_pair((int)'"', 1);
	for(int meta = 0; meta < 128; ++ meta)
	{
		if(meta == '\\') ss[1][meta] = std::make_pair(meta, 2);
		else if(meta == '"') ss[1][meta] = std::make_pair(meta, 3);
		else ss[1][meta] = std::make_pair(meta, 1);
		ss[2][meta] = std::make_pair(meta, 1);
	}
	instance().machines["string"] = state_machine(strsheet);
	}

	// set separators
	std::string tmp = "+-*/&|~^%()[]{}'\"\\,.;?<>=!@$#:";
	instance().separators.reset(tmp.size());
	for(size_t i = 0; i < tmp.size(); ++ i)
		instance().separators[i] = tmp[i];
}
