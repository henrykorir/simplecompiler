
#include <compiler.h>
#include <lalr1machine.h>
#include <statemachine.h>

using namespace sc;
using namespace compile;
using namespace compile::doc;

void init_machines(std::map<std::string, machine>& machines)
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
	machines["number"] = machine(kog::shared_ptr<automachine>(new state_machine(numsheet)), 0);
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
	;
	machines["symbol"] = machine(kog::shared_ptr<automachine>(new state_machine(symsheet)), 1);
	}
	
	{
		// init string machine
	state_machine::shared_sheet strsheet(new state_machine::sparsesheet());
	state_machine::sparsesheet& ss = *strsheet;
	
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
	machines["string"] = machine(kog::shared_ptr<automachine>(new state_machine(strsheet)), 2);
	}
}

void init_printablechars(kog::smart_vector<sc::int32>& printablechars)
{
	const std::string tmp = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
	printablechars.reset(tmp.size());
	for(size_t i = 0; i < tmp.size(); ++ i) printablechars[i] = tmp[i];
}

void init_syntax_machine(lalr1machine& lrm)
{
}

void init_keywords(kog::buckethash<std::string, int32, string_2_int>& keywords)
{
}

void init_separators(kog::smart_vector<sc::int32>& separators, kog::tree<int32>& sepsid)
{
	// set separators
	std::string tmp = "+-*/&|~^%()[]{}'\"\\,.;?<>=!@$#:";
	separators.reset(tmp.size());
	for(size_t i = 0; i < tmp.size(); ++ i)
		separators[i] = tmp[i];
}

void init_grammar(tinygrammar& tig)
{
}