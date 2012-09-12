#include <lalr1machine.h>
#include <compiler.h>
#include <type.h>
#include <treemaker.h>
#include <deque>

using namespace sc;
using namespace compile;
using namespace compile::doc;

void init_grammar(tinygrammar& tig)
{
	//// create symbols list: 
	std::deque<symbol> slist;
	slist.resize(33);
	{
		slist[0].sid = 0;
		slist[0].funcs = NULL;
		slist[0].Lname = 7;
		slist[0].Lfuncs = 0;
		slist[0].ist = 0;
		slist[0].name = "Program";
		
		slist[1].sid = 1;
		slist[1].funcs = NULL;
		slist[1].Lname = 12;
		slist[1].Lfuncs = 0;
		slist[1].ist = 0;
		slist[1].name = "AProgramItem";
		
		slist[2].sid = 2;
		slist[2].funcs = NULL;
		slist[2].Lname = 12;
		slist[2].Lfuncs = 0;
		slist[2].ist = 0;
		slist[2].name = "ValueDeclear";
		
		slist[3].sid = 3;
		slist[3].funcs = NULL;
		slist[3].Lname = 8;
		slist[3].Lfuncs = 0;
		slist[3].ist = 0;
		slist[3].name = "Function";
		
		slist[4].sid = 4;
		slist[4].funcs = NULL;
		slist[4].Lname = 11;
		slist[4].Lfuncs = 0;
		slist[4].ist = 0;
		slist[4].name = "Expressions";
		
		slist[5].sid = 5;
		slist[5].funcs = NULL;
		slist[5].Lname = 9;
		slist[5].Lfuncs = 0;
		slist[5].ist = 0;
		slist[5].name = "Valuetype";
		
		slist[6].sid = 6;
		slist[6].funcs = NULL;
		slist[6].Lname = 6;
		slist[6].Lfuncs = 0;
		slist[6].ist = 1;
		slist[6].name = "symbol";
		
		slist[7].sid = 7;
		slist[7].funcs = NULL;
		slist[7].Lname = 11;
		slist[7].Lfuncs = 0;
		slist[7].ist = 0;
		slist[7].name = "AExpression";
		
		slist[8].sid = 8;
		slist[8].funcs = NULL;
		slist[8].Lname = 6;
		slist[8].Lfuncs = 0;
		slist[8].ist = 0;
		slist[8].name = "AValue";
		
		slist[9].sid = 9;
		slist[9].funcs = NULL;
		slist[9].Lname = 6;
		slist[9].Lfuncs = 0;
		slist[9].ist = 1;
		slist[9].name = "number";
		
		slist[10].sid = 10;
		slist[10].funcs = NULL;
		slist[10].Lname = 8;
		slist[10].Lfuncs = 0;
		slist[10].ist = 0;
		slist[10].name = "FuncCall";
		
		slist[11].sid = 11;
		slist[11].funcs = NULL;
		slist[11].Lname = 2;
		slist[11].Lfuncs = 0;
		slist[11].ist = 0;
		slist[11].name = "Op";
		
		slist[12].sid = 12;
		slist[12].funcs = NULL;
		slist[12].Lname = 9;
		slist[12].Lfuncs = 0;
		slist[12].ist = 0;
		slist[12].name = "PrintFunc";
		
		slist[13].sid = 13;
		slist[13].funcs = NULL;
		slist[13].Lname = 5;
		slist[13].Lfuncs = 0;
		slist[13].ist = 1;
		slist[13].name = "print";
		
		slist[14].sid = 14;
		slist[14].funcs = NULL;
		slist[14].Lname = 15;
		slist[14].Lfuncs = 0;
		slist[14].ist = 0;
		slist[14].name = "FunctionContent";
		
		slist[15].sid = 15;
		slist[15].funcs = NULL;
		slist[15].Lname = 9;
		slist[15].Lfuncs = 0;
		slist[15].ist = 0;
		slist[15].name = "ReturnExp";
		
		slist[16].sid = 16;
		slist[16].funcs = NULL;
		slist[16].Lname = 6;
		slist[16].Lfuncs = 0;
		slist[16].ist = 1;
		slist[16].name = "return";
		
		slist[17].sid = 17;
		slist[17].funcs = NULL;
		slist[17].Lname = 3;
		slist[17].Lfuncs = 0;
		slist[17].ist = 1;
		slist[17].name = "int";
		
		slist[18].sid = 18;
		slist[18].funcs = NULL;
		slist[18].Lname = 5;
		slist[18].Lfuncs = 0;
		slist[18].ist = 1;
		slist[18].name = "float";
		
		slist[19].sid = 19;
		slist[19].funcs = NULL;
		slist[19].Lname = 10;
		slist[19].Lfuncs = 0;
		slist[19].ist = 0;
		slist[19].name = "Assignment";
		
		slist[20].sid = 20;
		slist[20].funcs = NULL;
		slist[20].Lname = 14;
		slist[20].Lfuncs = 0;
		slist[20].ist = 0;
		slist[20].name = "FunctionHeader";
		
		slist[21].sid = 21;
		slist[21].funcs = NULL;
		slist[21].Lname = 13;
		slist[21].Lfuncs = 0;
		slist[21].ist = 0;
		slist[21].name = "FunctionBegin";
		
		slist[22].sid = 22;
		slist[22].funcs = NULL;
		slist[22].Lname = 1;
		slist[22].Lfuncs = 0;
		slist[22].ist = 1;
		slist[22].name = "{";
		
		slist[23].sid = 23;
		slist[23].funcs = NULL;
		slist[23].Lname = 1;
		slist[23].Lfuncs = 0;
		slist[23].ist = 1;
		slist[23].name = "}";
		
		slist[24].sid = 24;
		slist[24].funcs = NULL;
		slist[24].Lname = 1;
		slist[24].Lfuncs = 0;
		slist[24].ist = 1;
		slist[24].name = "(";
		
		slist[25].sid = 25;
		slist[25].funcs = NULL;
		slist[25].Lname = 1;
		slist[25].Lfuncs = 0;
		slist[25].ist = 1;
		slist[25].name = ")";
		
		slist[26].sid = 26;
		slist[26].funcs = NULL;
		slist[26].Lname = 1;
		slist[26].Lfuncs = 0;
		slist[26].ist = 1;
		slist[26].name = "=";
		
		slist[27].sid = 27;
		slist[27].funcs = NULL;
		slist[27].Lname = 1;
		slist[27].Lfuncs = 0;
		slist[27].ist = 1;
		slist[27].name = "*";
		
		slist[28].sid = 28;
		slist[28].funcs = NULL;
		slist[28].Lname = 1;
		slist[28].Lfuncs = 0;
		slist[28].ist = 1;
		slist[28].name = "+";
		
		slist[29].sid = 29;
		slist[29].funcs = NULL;
		slist[29].Lname = 1;
		slist[29].Lfuncs = 0;
		slist[29].ist = 1;
		slist[29].name = "-";
		
		slist[30].sid = 30;
		slist[30].funcs = NULL;
		slist[30].Lname = 1;
		slist[30].Lfuncs = 0;
		slist[30].ist = 1;
		slist[30].name = "/";
		
		slist[31].sid = 31;
		slist[31].funcs = NULL;
		slist[31].Lname = 1;
		slist[31].Lfuncs = 0;
		slist[31].ist = 1;
		slist[31].name = ";";
		
		slist[32].sid = 32;
		slist[32].funcs = NULL;
		slist[32].Lname = 1;
		slist[32].Lfuncs = 0;
		slist[32].ist = 1;
		slist[32].name = ",";
		
	}
	symholder tmpholder(slist.begin(), slist.end());
	tmpholder.make_index();
	//// create productions
	tinygrammar::vecprods plist;
	plist.reset(29);
	{
		int32 tmpv_0[] = {1};
		plist[0] = production(0, tmpv_0, sizeof(tmpv_0)/sizeof(int32));
		
		int32 tmpv_1[] = {1, 0};
		plist[1] = production(0, tmpv_1, sizeof(tmpv_1)/sizeof(int32));
		
		int32 tmpv_2[] = {3};
		plist[2] = production(1, tmpv_2, sizeof(tmpv_2)/sizeof(int32));
		
		int32 tmpv_3[] = {7, 31};
		plist[3] = production(1, tmpv_3, sizeof(tmpv_3)/sizeof(int32));
		
		int32 tmpv_4[] = {5, 6};
		plist[4] = production(2, tmpv_4, sizeof(tmpv_4)/sizeof(int32));
		
		int32 tmpv_5[] = {7, 31, 4};
		plist[5] = production(4, tmpv_5, sizeof(tmpv_5)/sizeof(int32));
		
		int32 tmpv_6[] = {7, 31};
		plist[6] = production(4, tmpv_6, sizeof(tmpv_6)/sizeof(int32));
		
		int32 tmpv_7[] = {2};
		plist[7] = production(7, tmpv_7, sizeof(tmpv_7)/sizeof(int32));
		
		int32 tmpv_8[] = {10};
		plist[8] = production(7, tmpv_8, sizeof(tmpv_8)/sizeof(int32));
		
		int32 tmpv_9[] = {12};
		plist[9] = production(7, tmpv_9, sizeof(tmpv_9)/sizeof(int32));
		
		int32 tmpv_10[] = {19};
		plist[10] = production(7, tmpv_10, sizeof(tmpv_10)/sizeof(int32));
		
		int32 tmpv_11[] = {6, 26, 8};
		plist[11] = production(19, tmpv_11, sizeof(tmpv_11)/sizeof(int32));
		
		int32 tmpv_12[] = {6, 26, 8, 11, 8};
		plist[12] = production(19, tmpv_12, sizeof(tmpv_12)/sizeof(int32));
		
		int32 tmpv_13[] = {6};
		plist[13] = production(8, tmpv_13, sizeof(tmpv_13)/sizeof(int32));
		
		int32 tmpv_14[] = {9};
		plist[14] = production(8, tmpv_14, sizeof(tmpv_14)/sizeof(int32));
		
		int32 tmpv_15[] = {6, 26, 6, 24, 6, 32, 6, 25};
		plist[15] = production(10, tmpv_15, sizeof(tmpv_15)/sizeof(int32));
		
		int32 tmpv_16[] = {13, 24, 8, 25};
		plist[16] = production(12, tmpv_16, sizeof(tmpv_16)/sizeof(int32));
		
		int32 tmpv_17[] = {21, 14, 15, 23};
		plist[17] = production(3, tmpv_17, sizeof(tmpv_17)/sizeof(int32));
		
		int32 tmpv_18[] = {21, 15, 23};
		plist[18] = production(3, tmpv_18, sizeof(tmpv_18)/sizeof(int32));
		
		int32 tmpv_19[] = {4};
		plist[19] = production(14, tmpv_19, sizeof(tmpv_19)/sizeof(int32));
		
		int32 tmpv_20[] = {16, 8, 31};
		plist[20] = production(15, tmpv_20, sizeof(tmpv_20)/sizeof(int32));
		
		int32 tmpv_21[] = {17};
		plist[21] = production(5, tmpv_21, sizeof(tmpv_21)/sizeof(int32));
		
		int32 tmpv_22[] = {18};
		plist[22] = production(5, tmpv_22, sizeof(tmpv_22)/sizeof(int32));
		
		int32 tmpv_23[] = {28};
		plist[23] = production(11, tmpv_23, sizeof(tmpv_23)/sizeof(int32));
		
		int32 tmpv_24[] = {29};
		plist[24] = production(11, tmpv_24, sizeof(tmpv_24)/sizeof(int32));
		
		int32 tmpv_25[] = {27};
		plist[25] = production(11, tmpv_25, sizeof(tmpv_25)/sizeof(int32));
		
		int32 tmpv_26[] = {30};
		plist[26] = production(11, tmpv_26, sizeof(tmpv_26)/sizeof(int32));
		
		int32 tmpv_27[] = {5, 6, 24, 5, 6, 32, 5, 6, 25};
		plist[27] = production(20, tmpv_27, sizeof(tmpv_27)/sizeof(int32));
		
		int32 tmpv_28[] = {20, 22};
		plist[28] = production(21, tmpv_28, sizeof(tmpv_28)/sizeof(int32));
		
	}
	
	tig.symbols().swap(tmpholder);
	tig.productions().swap(plist);
	tig.starts() = 0;
	tig.eplisons() = -1;
	tig.endings() = -1;
	
}

class tree_wrap : public make_tree
{
public:
	void make()
	{
		link r = new_node();
		r->children['{']->v = 22;
		r->children['}']->v = 23;
		r->children['(']->v = 24;
		r->children[')']->v = 25;
		r->children['=']->v = 26;
		r->children['*']->v = 27;
		r->children['+']->v = 28;
		r->children['-']->v = 29;
		r->children['/']->v = 30;
		r->children[';']->v = 31;
		r->children[',']->v = 32;
		reset_root(r);
	}
	

};
void init_separators(kog::smart_vector<sc::int32>& separators, kog::tree<int32>& sepsid)
{
	const char tmp[] = "{}()=*+-/;,";
	separators.reset(sizeof(tmp));
	std::copy(tmp, tmp + sizeof(tmp), separators.begin());
	//// create tree
	tree_wrap* tw = reinterpret_cast<tree_wrap*>(&sepsid);
	tw->make();
	
}
void init_keywords(kog::buckethash<std::string, int32, string_2_int>& keywords)
{
	keywords.insert("print", 13);
	keywords.insert("return", 16);
	keywords.insert("int", 17);
	keywords.insert("float", 18);
	
}

void init_machines(std::map<std::string, machine>& machines)
{
	
	{
		kog::shared_ptr<automachine> ptr_m(new state_machine);
		automachine& m = *ptr_m;
		automachine::sparsesheet& sheet = m.sheet();
		sheet.reset(2);
		automachine::sheetrow::value_type* ptr = NULL;
		
		sheet[0].reset(63);
		sheet[0].type(1);
		sheet[0].endings(1);
		ptr = sheet[0].get();
			*(ptr ++) = std::make_pair(48, 0);
			*(ptr ++) = std::make_pair(49, 0);
			*(ptr ++) = std::make_pair(50, 0);
			*(ptr ++) = std::make_pair(51, 0);
			*(ptr ++) = std::make_pair(52, 0);
			*(ptr ++) = std::make_pair(53, 0);
			*(ptr ++) = std::make_pair(54, 0);
			*(ptr ++) = std::make_pair(55, 0);
			*(ptr ++) = std::make_pair(56, 0);
			*(ptr ++) = std::make_pair(57, 0);
			*(ptr ++) = std::make_pair(65, 0);
			*(ptr ++) = std::make_pair(66, 0);
			*(ptr ++) = std::make_pair(67, 0);
			*(ptr ++) = std::make_pair(68, 0);
			*(ptr ++) = std::make_pair(69, 0);
			*(ptr ++) = std::make_pair(70, 0);
			*(ptr ++) = std::make_pair(71, 0);
			*(ptr ++) = std::make_pair(72, 0);
			*(ptr ++) = std::make_pair(73, 0);
			*(ptr ++) = std::make_pair(74, 0);
			*(ptr ++) = std::make_pair(75, 0);
			*(ptr ++) = std::make_pair(76, 0);
			*(ptr ++) = std::make_pair(77, 0);
			*(ptr ++) = std::make_pair(78, 0);
			*(ptr ++) = std::make_pair(79, 0);
			*(ptr ++) = std::make_pair(80, 0);
			*(ptr ++) = std::make_pair(81, 0);
			*(ptr ++) = std::make_pair(82, 0);
			*(ptr ++) = std::make_pair(83, 0);
			*(ptr ++) = std::make_pair(84, 0);
			*(ptr ++) = std::make_pair(85, 0);
			*(ptr ++) = std::make_pair(86, 0);
			*(ptr ++) = std::make_pair(87, 0);
			*(ptr ++) = std::make_pair(88, 0);
			*(ptr ++) = std::make_pair(89, 0);
			*(ptr ++) = std::make_pair(90, 0);
			*(ptr ++) = std::make_pair(95, 0);
			*(ptr ++) = std::make_pair(97, 0);
			*(ptr ++) = std::make_pair(98, 0);
			*(ptr ++) = std::make_pair(99, 0);
			*(ptr ++) = std::make_pair(100, 0);
			*(ptr ++) = std::make_pair(101, 0);
			*(ptr ++) = std::make_pair(102, 0);
			*(ptr ++) = std::make_pair(103, 0);
			*(ptr ++) = std::make_pair(104, 0);
			*(ptr ++) = std::make_pair(105, 0);
			*(ptr ++) = std::make_pair(106, 0);
			*(ptr ++) = std::make_pair(107, 0);
			*(ptr ++) = std::make_pair(108, 0);
			*(ptr ++) = std::make_pair(109, 0);
			*(ptr ++) = std::make_pair(110, 0);
			*(ptr ++) = std::make_pair(111, 0);
			*(ptr ++) = std::make_pair(112, 0);
			*(ptr ++) = std::make_pair(113, 0);
			*(ptr ++) = std::make_pair(114, 0);
			*(ptr ++) = std::make_pair(115, 0);
			*(ptr ++) = std::make_pair(116, 0);
			*(ptr ++) = std::make_pair(117, 0);
			*(ptr ++) = std::make_pair(118, 0);
			*(ptr ++) = std::make_pair(119, 0);
			*(ptr ++) = std::make_pair(120, 0);
			*(ptr ++) = std::make_pair(121, 0);
			*(ptr ++) = std::make_pair(122, 0);
		
		sheet[1].reset(53);
		sheet[1].type(1);
		sheet[1].endings(0);
		ptr = sheet[1].get();
			*(ptr ++) = std::make_pair(65, 0);
			*(ptr ++) = std::make_pair(66, 0);
			*(ptr ++) = std::make_pair(67, 0);
			*(ptr ++) = std::make_pair(68, 0);
			*(ptr ++) = std::make_pair(69, 0);
			*(ptr ++) = std::make_pair(70, 0);
			*(ptr ++) = std::make_pair(71, 0);
			*(ptr ++) = std::make_pair(72, 0);
			*(ptr ++) = std::make_pair(73, 0);
			*(ptr ++) = std::make_pair(74, 0);
			*(ptr ++) = std::make_pair(75, 0);
			*(ptr ++) = std::make_pair(76, 0);
			*(ptr ++) = std::make_pair(77, 0);
			*(ptr ++) = std::make_pair(78, 0);
			*(ptr ++) = std::make_pair(79, 0);
			*(ptr ++) = std::make_pair(80, 0);
			*(ptr ++) = std::make_pair(81, 0);
			*(ptr ++) = std::make_pair(82, 0);
			*(ptr ++) = std::make_pair(83, 0);
			*(ptr ++) = std::make_pair(84, 0);
			*(ptr ++) = std::make_pair(85, 0);
			*(ptr ++) = std::make_pair(86, 0);
			*(ptr ++) = std::make_pair(87, 0);
			*(ptr ++) = std::make_pair(88, 0);
			*(ptr ++) = std::make_pair(89, 0);
			*(ptr ++) = std::make_pair(90, 0);
			*(ptr ++) = std::make_pair(95, 0);
			*(ptr ++) = std::make_pair(97, 0);
			*(ptr ++) = std::make_pair(98, 0);
			*(ptr ++) = std::make_pair(99, 0);
			*(ptr ++) = std::make_pair(100, 0);
			*(ptr ++) = std::make_pair(101, 0);
			*(ptr ++) = std::make_pair(102, 0);
			*(ptr ++) = std::make_pair(103, 0);
			*(ptr ++) = std::make_pair(104, 0);
			*(ptr ++) = std::make_pair(105, 0);
			*(ptr ++) = std::make_pair(106, 0);
			*(ptr ++) = std::make_pair(107, 0);
			*(ptr ++) = std::make_pair(108, 0);
			*(ptr ++) = std::make_pair(109, 0);
			*(ptr ++) = std::make_pair(110, 0);
			*(ptr ++) = std::make_pair(111, 0);
			*(ptr ++) = std::make_pair(112, 0);
			*(ptr ++) = std::make_pair(113, 0);
			*(ptr ++) = std::make_pair(114, 0);
			*(ptr ++) = std::make_pair(115, 0);
			*(ptr ++) = std::make_pair(116, 0);
			*(ptr ++) = std::make_pair(117, 0);
			*(ptr ++) = std::make_pair(118, 0);
			*(ptr ++) = std::make_pair(119, 0);
			*(ptr ++) = std::make_pair(120, 0);
			*(ptr ++) = std::make_pair(121, 0);
			*(ptr ++) = std::make_pair(122, 0);
		
		m.sstate() = 1;
		machines["symbol"] = machine(ptr_m, 6);
	}
	
	{
		kog::shared_ptr<automachine> ptr_m(new state_machine);
		automachine& m = *ptr_m;
		automachine::sparsesheet& sheet = m.sheet();
		sheet.reset(4);
		automachine::sheetrow::value_type* ptr = NULL;
		
		sheet[0].reset(10);
		sheet[0].type(1);
		sheet[0].endings(1);
		ptr = sheet[0].get();
			*(ptr ++) = std::make_pair(48, 0);
			*(ptr ++) = std::make_pair(49, 0);
			*(ptr ++) = std::make_pair(50, 0);
			*(ptr ++) = std::make_pair(51, 0);
			*(ptr ++) = std::make_pair(52, 0);
			*(ptr ++) = std::make_pair(53, 0);
			*(ptr ++) = std::make_pair(54, 0);
			*(ptr ++) = std::make_pair(55, 0);
			*(ptr ++) = std::make_pair(56, 0);
			*(ptr ++) = std::make_pair(57, 0);
		
		sheet[1].reset(10);
		sheet[1].type(1);
		sheet[1].endings(0);
		ptr = sheet[1].get();
			*(ptr ++) = std::make_pair(48, 0);
			*(ptr ++) = std::make_pair(49, 0);
			*(ptr ++) = std::make_pair(50, 0);
			*(ptr ++) = std::make_pair(51, 0);
			*(ptr ++) = std::make_pair(52, 0);
			*(ptr ++) = std::make_pair(53, 0);
			*(ptr ++) = std::make_pair(54, 0);
			*(ptr ++) = std::make_pair(55, 0);
			*(ptr ++) = std::make_pair(56, 0);
			*(ptr ++) = std::make_pair(57, 0);
		
		sheet[2].reset(11);
		sheet[2].type(1);
		sheet[2].endings(1);
		ptr = sheet[2].get();
			*(ptr ++) = std::make_pair(46, 0);
			*(ptr ++) = std::make_pair(48, 2);
			*(ptr ++) = std::make_pair(49, 2);
			*(ptr ++) = std::make_pair(50, 2);
			*(ptr ++) = std::make_pair(51, 2);
			*(ptr ++) = std::make_pair(52, 2);
			*(ptr ++) = std::make_pair(53, 2);
			*(ptr ++) = std::make_pair(54, 2);
			*(ptr ++) = std::make_pair(55, 2);
			*(ptr ++) = std::make_pair(56, 2);
			*(ptr ++) = std::make_pair(57, 2);
		
		sheet[3].reset(11);
		sheet[3].type(1);
		sheet[3].endings(0);
		ptr = sheet[3].get();
			*(ptr ++) = std::make_pair(46, 1);
			*(ptr ++) = std::make_pair(48, 2);
			*(ptr ++) = std::make_pair(49, 2);
			*(ptr ++) = std::make_pair(50, 2);
			*(ptr ++) = std::make_pair(51, 2);
			*(ptr ++) = std::make_pair(52, 2);
			*(ptr ++) = std::make_pair(53, 2);
			*(ptr ++) = std::make_pair(54, 2);
			*(ptr ++) = std::make_pair(55, 2);
			*(ptr ++) = std::make_pair(56, 2);
			*(ptr ++) = std::make_pair(57, 2);
		
		m.sstate() = 3;
		machines["number"] = machine(ptr_m, 9);
	}
	
}

void init_syntax_machine(lalr1machine& lrm)
{
	kog::shared_ptr<automachine> ptr_m(new state_machine);
	automachine& m = *ptr_m;
	automachine::sparsesheet& sheet = m.sheet();
	sheet.reset(67);
	automachine::sheetrow::value_type* ptr = NULL;
	
	sheet[0].reset(0);
	sheet[0].type(1);
	sheet[0].endings(1);
	ptr = sheet[0].get();
	
	sheet[1].reset(15);
	sheet[1].type(1);
	sheet[1].endings(0);
	ptr = sheet[1].get();
		*(ptr ++) = std::make_pair(0, 0);
		*(ptr ++) = std::make_pair(1, 2);
		*(ptr ++) = std::make_pair(2, 3);
		*(ptr ++) = std::make_pair(3, 4);
		*(ptr ++) = std::make_pair(5, 5);
		*(ptr ++) = std::make_pair(6, 6);
		*(ptr ++) = std::make_pair(7, 7);
		*(ptr ++) = std::make_pair(10, 8);
		*(ptr ++) = std::make_pair(12, 9);
		*(ptr ++) = std::make_pair(13, 10);
		*(ptr ++) = std::make_pair(17, 11);
		*(ptr ++) = std::make_pair(18, 12);
		*(ptr ++) = std::make_pair(19, 13);
		*(ptr ++) = std::make_pair(20, 14);
		*(ptr ++) = std::make_pair(21, 15);
	
	sheet[2].reset(16);
	sheet[2].type(1);
	sheet[2].endings(0);
	ptr = sheet[2].get();
		*(ptr ++) = std::make_pair(-1, -1);
		*(ptr ++) = std::make_pair(0, 16);
		*(ptr ++) = std::make_pair(1, 17);
		*(ptr ++) = std::make_pair(2, 3);
		*(ptr ++) = std::make_pair(3, 4);
		*(ptr ++) = std::make_pair(5, 5);
		*(ptr ++) = std::make_pair(6, 6);
		*(ptr ++) = std::make_pair(7, 7);
		*(ptr ++) = std::make_pair(10, 8);
		*(ptr ++) = std::make_pair(12, 9);
		*(ptr ++) = std::make_pair(13, 10);
		*(ptr ++) = std::make_pair(17, 11);
		*(ptr ++) = std::make_pair(18, 12);
		*(ptr ++) = std::make_pair(19, 13);
		*(ptr ++) = std::make_pair(20, 14);
		*(ptr ++) = std::make_pair(21, 15);
	
	sheet[3].reset(1);
	sheet[3].type(1);
	sheet[3].endings(0);
	ptr = sheet[3].get();
		*(ptr ++) = std::make_pair(31, -8);
	
	sheet[4].reset(5);
	sheet[4].type(1);
	sheet[4].endings(0);
	ptr = sheet[4].get();
		*(ptr ++) = std::make_pair(-1, -3);
		*(ptr ++) = std::make_pair(6, -3);
		*(ptr ++) = std::make_pair(13, -3);
		*(ptr ++) = std::make_pair(17, -3);
		*(ptr ++) = std::make_pair(18, -3);
	
	sheet[5].reset(1);
	sheet[5].type(1);
	sheet[5].endings(0);
	ptr = sheet[5].get();
		*(ptr ++) = std::make_pair(6, 18);
	
	sheet[6].reset(1);
	sheet[6].type(1);
	sheet[6].endings(0);
	ptr = sheet[6].get();
		*(ptr ++) = std::make_pair(26, 19);
	
	sheet[7].reset(1);
	sheet[7].type(1);
	sheet[7].endings(0);
	ptr = sheet[7].get();
		*(ptr ++) = std::make_pair(31, 20);
	
	sheet[8].reset(1);
	sheet[8].type(1);
	sheet[8].endings(0);
	ptr = sheet[8].get();
		*(ptr ++) = std::make_pair(31, -9);
	
	sheet[9].reset(1);
	sheet[9].type(1);
	sheet[9].endings(0);
	ptr = sheet[9].get();
		*(ptr ++) = std::make_pair(31, -10);
	
	sheet[10].reset(1);
	sheet[10].type(1);
	sheet[10].endings(0);
	ptr = sheet[10].get();
		*(ptr ++) = std::make_pair(24, 21);
	
	sheet[11].reset(1);
	sheet[11].type(1);
	sheet[11].endings(0);
	ptr = sheet[11].get();
		*(ptr ++) = std::make_pair(6, -22);
	
	sheet[12].reset(1);
	sheet[12].type(1);
	sheet[12].endings(0);
	ptr = sheet[12].get();
		*(ptr ++) = std::make_pair(6, -23);
	
	sheet[13].reset(1);
	sheet[13].type(1);
	sheet[13].endings(0);
	ptr = sheet[13].get();
		*(ptr ++) = std::make_pair(31, -11);
	
	sheet[14].reset(1);
	sheet[14].type(1);
	sheet[14].endings(0);
	ptr = sheet[14].get();
		*(ptr ++) = std::make_pair(22, 22);
	
	sheet[15].reset(14);
	sheet[15].type(1);
	sheet[15].endings(0);
	ptr = sheet[15].get();
		*(ptr ++) = std::make_pair(2, 3);
		*(ptr ++) = std::make_pair(4, 23);
		*(ptr ++) = std::make_pair(5, 24);
		*(ptr ++) = std::make_pair(6, 25);
		*(ptr ++) = std::make_pair(7, 26);
		*(ptr ++) = std::make_pair(10, 8);
		*(ptr ++) = std::make_pair(12, 9);
		*(ptr ++) = std::make_pair(13, 10);
		*(ptr ++) = std::make_pair(14, 27);
		*(ptr ++) = std::make_pair(15, 28);
		*(ptr ++) = std::make_pair(16, 29);
		*(ptr ++) = std::make_pair(17, 11);
		*(ptr ++) = std::make_pair(18, 12);
		*(ptr ++) = std::make_pair(19, 13);
	
	sheet[16].reset(1);
	sheet[16].type(1);
	sheet[16].endings(0);
	ptr = sheet[16].get();
		*(ptr ++) = std::make_pair(-1, -2);
	
	sheet[17].reset(16);
	sheet[17].type(1);
	sheet[17].endings(0);
	ptr = sheet[17].get();
		*(ptr ++) = std::make_pair(-1, -1);
		*(ptr ++) = std::make_pair(0, 16);
		*(ptr ++) = std::make_pair(1, 17);
		*(ptr ++) = std::make_pair(2, 3);
		*(ptr ++) = std::make_pair(3, 4);
		*(ptr ++) = std::make_pair(5, 5);
		*(ptr ++) = std::make_pair(6, 6);
		*(ptr ++) = std::make_pair(7, 7);
		*(ptr ++) = std::make_pair(10, 8);
		*(ptr ++) = std::make_pair(12, 9);
		*(ptr ++) = std::make_pair(13, 10);
		*(ptr ++) = std::make_pair(17, 11);
		*(ptr ++) = std::make_pair(18, 12);
		*(ptr ++) = std::make_pair(19, 13);
		*(ptr ++) = std::make_pair(20, 14);
		*(ptr ++) = std::make_pair(21, 15);
	
	sheet[18].reset(2);
	sheet[18].type(1);
	sheet[18].endings(0);
	ptr = sheet[18].get();
		*(ptr ++) = std::make_pair(24, 30);
		*(ptr ++) = std::make_pair(31, -5);
	
	sheet[19].reset(3);
	sheet[19].type(1);
	sheet[19].endings(0);
	ptr = sheet[19].get();
		*(ptr ++) = std::make_pair(6, 31);
		*(ptr ++) = std::make_pair(8, 32);
		*(ptr ++) = std::make_pair(9, 33);
	
	sheet[20].reset(5);
	sheet[20].type(1);
	sheet[20].endings(0);
	ptr = sheet[20].get();
		*(ptr ++) = std::make_pair(-1, -4);
		*(ptr ++) = std::make_pair(6, -4);
		*(ptr ++) = std::make_pair(13, -4);
		*(ptr ++) = std::make_pair(17, -4);
		*(ptr ++) = std::make_pair(18, -4);
	
	sheet[21].reset(3);
	sheet[21].type(1);
	sheet[21].endings(0);
	ptr = sheet[21].get();
		*(ptr ++) = std::make_pair(6, 34);
		*(ptr ++) = std::make_pair(8, 35);
		*(ptr ++) = std::make_pair(9, 36);
	
	sheet[22].reset(5);
	sheet[22].type(1);
	sheet[22].endings(0);
	ptr = sheet[22].get();
		*(ptr ++) = std::make_pair(6, -29);
		*(ptr ++) = std::make_pair(13, -29);
		*(ptr ++) = std::make_pair(16, -29);
		*(ptr ++) = std::make_pair(17, -29);
		*(ptr ++) = std::make_pair(18, -29);
	
	sheet[23].reset(1);
	sheet[23].type(1);
	sheet[23].endings(0);
	ptr = sheet[23].get();
		*(ptr ++) = std::make_pair(16, -20);
	
	sheet[24].reset(1);
	sheet[24].type(1);
	sheet[24].endings(0);
	ptr = sheet[24].get();
		*(ptr ++) = std::make_pair(6, 37);
	
	sheet[25].reset(1);
	sheet[25].type(1);
	sheet[25].endings(0);
	ptr = sheet[25].get();
		*(ptr ++) = std::make_pair(26, 38);
	
	sheet[26].reset(1);
	sheet[26].type(1);
	sheet[26].endings(0);
	ptr = sheet[26].get();
		*(ptr ++) = std::make_pair(31, 39);
	
	sheet[27].reset(2);
	sheet[27].type(1);
	sheet[27].endings(0);
	ptr = sheet[27].get();
		*(ptr ++) = std::make_pair(15, 40);
		*(ptr ++) = std::make_pair(16, 29);
	
	sheet[28].reset(1);
	sheet[28].type(1);
	sheet[28].endings(0);
	ptr = sheet[28].get();
		*(ptr ++) = std::make_pair(23, 41);
	
	sheet[29].reset(3);
	sheet[29].type(1);
	sheet[29].endings(0);
	ptr = sheet[29].get();
		*(ptr ++) = std::make_pair(6, 42);
		*(ptr ++) = std::make_pair(8, 43);
		*(ptr ++) = std::make_pair(9, 44);
	
	sheet[30].reset(3);
	sheet[30].type(1);
	sheet[30].endings(0);
	ptr = sheet[30].get();
		*(ptr ++) = std::make_pair(5, 45);
		*(ptr ++) = std::make_pair(17, 11);
		*(ptr ++) = std::make_pair(18, 12);
	
	sheet[31].reset(6);
	sheet[31].type(1);
	sheet[31].endings(0);
	ptr = sheet[31].get();
		*(ptr ++) = std::make_pair(24, 46);
		*(ptr ++) = std::make_pair(27, -14);
		*(ptr ++) = std::make_pair(28, -14);
		*(ptr ++) = std::make_pair(29, -14);
		*(ptr ++) = std::make_pair(30, -14);
		*(ptr ++) = std::make_pair(31, -14);
	
	sheet[32].reset(6);
	sheet[32].type(1);
	sheet[32].endings(0);
	ptr = sheet[32].get();
		*(ptr ++) = std::make_pair(11, 47);
		*(ptr ++) = std::make_pair(27, 48);
		*(ptr ++) = std::make_pair(28, 49);
		*(ptr ++) = std::make_pair(29, 50);
		*(ptr ++) = std::make_pair(30, 51);
		*(ptr ++) = std::make_pair(31, -12);
	
	sheet[33].reset(5);
	sheet[33].type(1);
	sheet[33].endings(0);
	ptr = sheet[33].get();
		*(ptr ++) = std::make_pair(27, -15);
		*(ptr ++) = std::make_pair(28, -15);
		*(ptr ++) = std::make_pair(29, -15);
		*(ptr ++) = std::make_pair(30, -15);
		*(ptr ++) = std::make_pair(31, -15);
	
	sheet[34].reset(1);
	sheet[34].type(1);
	sheet[34].endings(0);
	ptr = sheet[34].get();
		*(ptr ++) = std::make_pair(25, -14);
	
	sheet[35].reset(1);
	sheet[35].type(1);
	sheet[35].endings(0);
	ptr = sheet[35].get();
		*(ptr ++) = std::make_pair(25, 52);
	
	sheet[36].reset(1);
	sheet[36].type(1);
	sheet[36].endings(0);
	ptr = sheet[36].get();
		*(ptr ++) = std::make_pair(25, -15);
	
	sheet[37].reset(1);
	sheet[37].type(1);
	sheet[37].endings(0);
	ptr = sheet[37].get();
		*(ptr ++) = std::make_pair(31, -5);
	
	sheet[38].reset(3);
	sheet[38].type(1);
	sheet[38].endings(0);
	ptr = sheet[38].get();
		*(ptr ++) = std::make_pair(6, 31);
		*(ptr ++) = std::make_pair(8, 53);
		*(ptr ++) = std::make_pair(9, 33);
	
	sheet[39].reset(12);
	sheet[39].type(1);
	sheet[39].endings(0);
	ptr = sheet[39].get();
		*(ptr ++) = std::make_pair(2, 3);
		*(ptr ++) = std::make_pair(4, 54);
		*(ptr ++) = std::make_pair(5, 24);
		*(ptr ++) = std::make_pair(6, 6);
		*(ptr ++) = std::make_pair(7, 26);
		*(ptr ++) = std::make_pair(10, 8);
		*(ptr ++) = std::make_pair(12, 9);
		*(ptr ++) = std::make_pair(13, 10);
		*(ptr ++) = std::make_pair(16, -7);
		*(ptr ++) = std::make_pair(17, 11);
		*(ptr ++) = std::make_pair(18, 12);
		*(ptr ++) = std::make_pair(19, 13);
	
	sheet[40].reset(1);
	sheet[40].type(1);
	sheet[40].endings(0);
	ptr = sheet[40].get();
		*(ptr ++) = std::make_pair(23, 55);
	
	sheet[41].reset(5);
	sheet[41].type(1);
	sheet[41].endings(0);
	ptr = sheet[41].get();
		*(ptr ++) = std::make_pair(-1, -19);
		*(ptr ++) = std::make_pair(6, -19);
		*(ptr ++) = std::make_pair(13, -19);
		*(ptr ++) = std::make_pair(17, -19);
		*(ptr ++) = std::make_pair(18, -19);
	
	sheet[42].reset(1);
	sheet[42].type(1);
	sheet[42].endings(0);
	ptr = sheet[42].get();
		*(ptr ++) = std::make_pair(31, -14);
	
	sheet[43].reset(1);
	sheet[43].type(1);
	sheet[43].endings(0);
	ptr = sheet[43].get();
		*(ptr ++) = std::make_pair(31, 56);
	
	sheet[44].reset(1);
	sheet[44].type(1);
	sheet[44].endings(0);
	ptr = sheet[44].get();
		*(ptr ++) = std::make_pair(31, -15);
	
	sheet[45].reset(1);
	sheet[45].type(1);
	sheet[45].endings(0);
	ptr = sheet[45].get();
		*(ptr ++) = std::make_pair(6, 57);
	
	sheet[46].reset(1);
	sheet[46].type(1);
	sheet[46].endings(0);
	ptr = sheet[46].get();
		*(ptr ++) = std::make_pair(6, 58);
	
	sheet[47].reset(3);
	sheet[47].type(1);
	sheet[47].endings(0);
	ptr = sheet[47].get();
		*(ptr ++) = std::make_pair(6, 42);
		*(ptr ++) = std::make_pair(8, 59);
		*(ptr ++) = std::make_pair(9, 44);
	
	sheet[48].reset(2);
	sheet[48].type(1);
	sheet[48].endings(0);
	ptr = sheet[48].get();
		*(ptr ++) = std::make_pair(6, -26);
		*(ptr ++) = std::make_pair(9, -26);
	
	sheet[49].reset(2);
	sheet[49].type(1);
	sheet[49].endings(0);
	ptr = sheet[49].get();
		*(ptr ++) = std::make_pair(6, -24);
		*(ptr ++) = std::make_pair(9, -24);
	
	sheet[50].reset(2);
	sheet[50].type(1);
	sheet[50].endings(0);
	ptr = sheet[50].get();
		*(ptr ++) = std::make_pair(6, -25);
		*(ptr ++) = std::make_pair(9, -25);
	
	sheet[51].reset(2);
	sheet[51].type(1);
	sheet[51].endings(0);
	ptr = sheet[51].get();
		*(ptr ++) = std::make_pair(6, -27);
		*(ptr ++) = std::make_pair(9, -27);
	
	sheet[52].reset(1);
	sheet[52].type(1);
	sheet[52].endings(0);
	ptr = sheet[52].get();
		*(ptr ++) = std::make_pair(31, -17);
	
	sheet[53].reset(6);
	sheet[53].type(1);
	sheet[53].endings(0);
	ptr = sheet[53].get();
		*(ptr ++) = std::make_pair(11, 47);
		*(ptr ++) = std::make_pair(27, 48);
		*(ptr ++) = std::make_pair(28, 49);
		*(ptr ++) = std::make_pair(29, 50);
		*(ptr ++) = std::make_pair(30, 51);
		*(ptr ++) = std::make_pair(31, -12);
	
	sheet[54].reset(1);
	sheet[54].type(1);
	sheet[54].endings(0);
	ptr = sheet[54].get();
		*(ptr ++) = std::make_pair(16, -6);
	
	sheet[55].reset(5);
	sheet[55].type(1);
	sheet[55].endings(0);
	ptr = sheet[55].get();
		*(ptr ++) = std::make_pair(-1, -18);
		*(ptr ++) = std::make_pair(6, -18);
		*(ptr ++) = std::make_pair(13, -18);
		*(ptr ++) = std::make_pair(17, -18);
		*(ptr ++) = std::make_pair(18, -18);
	
	sheet[56].reset(1);
	sheet[56].type(1);
	sheet[56].endings(0);
	ptr = sheet[56].get();
		*(ptr ++) = std::make_pair(23, -21);
	
	sheet[57].reset(1);
	sheet[57].type(1);
	sheet[57].endings(0);
	ptr = sheet[57].get();
		*(ptr ++) = std::make_pair(32, 60);
	
	sheet[58].reset(1);
	sheet[58].type(1);
	sheet[58].endings(0);
	ptr = sheet[58].get();
		*(ptr ++) = std::make_pair(32, 61);
	
	sheet[59].reset(1);
	sheet[59].type(1);
	sheet[59].endings(0);
	ptr = sheet[59].get();
		*(ptr ++) = std::make_pair(31, -13);
	
	sheet[60].reset(3);
	sheet[60].type(1);
	sheet[60].endings(0);
	ptr = sheet[60].get();
		*(ptr ++) = std::make_pair(5, 62);
		*(ptr ++) = std::make_pair(17, 11);
		*(ptr ++) = std::make_pair(18, 12);
	
	sheet[61].reset(1);
	sheet[61].type(1);
	sheet[61].endings(0);
	ptr = sheet[61].get();
		*(ptr ++) = std::make_pair(6, 63);
	
	sheet[62].reset(1);
	sheet[62].type(1);
	sheet[62].endings(0);
	ptr = sheet[62].get();
		*(ptr ++) = std::make_pair(6, 64);
	
	sheet[63].reset(1);
	sheet[63].type(1);
	sheet[63].endings(0);
	ptr = sheet[63].get();
		*(ptr ++) = std::make_pair(25, 65);
	
	sheet[64].reset(1);
	sheet[64].type(1);
	sheet[64].endings(0);
	ptr = sheet[64].get();
		*(ptr ++) = std::make_pair(25, 66);
	
	sheet[65].reset(1);
	sheet[65].type(1);
	sheet[65].endings(0);
	ptr = sheet[65].get();
		*(ptr ++) = std::make_pair(31, -16);
	
	sheet[66].reset(1);
	sheet[66].type(1);
	sheet[66].endings(0);
	ptr = sheet[66].get();
		*(ptr ++) = std::make_pair(22, -28);
	
	m.sstate() = 1;
	m.swap(lrm);
	
}
void init_printablechars(kog::smart_vector<sc::int32>& printablechars)
{
	
}

struct production_func_0 : public ifunction
{
	/* overwrite */ virtual machine_meta* operator()(machine_meta*const* metas, int C, machine_meta* result)
	{
	}
};
struct production_func_1 : public ifunction
{
	/* overwrite */ virtual machine_meta* operator()(machine_meta*const* metas, int C, machine_meta* result)
	{
	}
};
struct production_func_2 : public ifunction
{
	/* overwrite */ virtual machine_meta* operator()(machine_meta*const* metas, int C, machine_meta* result)
	{
	}
};
struct production_func_3 : public ifunction
{
	/* overwrite */ virtual machine_meta* operator()(machine_meta*const* metas, int C, machine_meta* result)
	{
	}
};
struct production_func_4 : public ifunction
{
	/* overwrite */ virtual machine_meta* operator()(machine_meta*const* metas, int C, machine_meta* result)
	{
	}
};
struct production_func_5 : public ifunction
{
	/* overwrite */ virtual machine_meta* operator()(machine_meta*const* metas, int C, machine_meta* result)
	{
	}
};
struct production_func_6 : public ifunction
{
	/* overwrite */ virtual machine_meta* operator()(machine_meta*const* metas, int C, machine_meta* result)
	{
	}
};
struct production_func_7 : public ifunction
{
	/* overwrite */ virtual machine_meta* operator()(machine_meta*const* metas, int C, machine_meta* result)
	{
	}
};
struct production_func_8 : public ifunction
{
	/* overwrite */ virtual machine_meta* operator()(machine_meta*const* metas, int C, machine_meta* result)
	{
	}
};
struct production_func_9 : public ifunction
{
	/* overwrite */ virtual machine_meta* operator()(machine_meta*const* metas, int C, machine_meta* result)
	{
	}
};
struct production_func_10 : public ifunction
{
	/* overwrite */ virtual machine_meta* operator()(machine_meta*const* metas, int C, machine_meta* result)
	{
	}
};
struct production_func_11 : public ifunction
{
	/* overwrite */ virtual machine_meta* operator()(machine_meta*const* metas, int C, machine_meta* result)
	{
	}
};
struct production_func_12 : public ifunction
{
	/* overwrite */ virtual machine_meta* operator()(machine_meta*const* metas, int C, machine_meta* result)
	{
	}
};
struct production_func_13 : public ifunction
{
	/* overwrite */ virtual machine_meta* operator()(machine_meta*const* metas, int C, machine_meta* result)
	{
	}
};
struct production_func_14 : public ifunction
{
	/* overwrite */ virtual machine_meta* operator()(machine_meta*const* metas, int C, machine_meta* result)
	{
	}
};
struct production_func_15 : public ifunction
{
	/* overwrite */ virtual machine_meta* operator()(machine_meta*const* metas, int C, machine_meta* result)
	{
	}
};
struct production_func_16 : public ifunction
{
	/* overwrite */ virtual machine_meta* operator()(machine_meta*const* metas, int C, machine_meta* result)
	{
	}
};
struct production_func_17 : public ifunction
{
	/* overwrite */ virtual machine_meta* operator()(machine_meta*const* metas, int C, machine_meta* result)
	{
	}
};
struct production_func_18 : public ifunction
{
	/* overwrite */ virtual machine_meta* operator()(machine_meta*const* metas, int C, machine_meta* result)
	{
	}
};
struct production_func_19 : public ifunction
{
	/* overwrite */ virtual machine_meta* operator()(machine_meta*const* metas, int C, machine_meta* result)
	{
	}
};
struct production_func_20 : public ifunction
{
	/* overwrite */ virtual machine_meta* operator()(machine_meta*const* metas, int C, machine_meta* result)
	{
	}
};
struct production_func_21 : public ifunction
{
	/* overwrite */ virtual machine_meta* operator()(machine_meta*const* metas, int C, machine_meta* result)
	{
	}
};
struct production_func_22 : public ifunction
{
	/* overwrite */ virtual machine_meta* operator()(machine_meta*const* metas, int C, machine_meta* result)
	{
	}
};
struct production_func_23 : public ifunction
{
	/* overwrite */ virtual machine_meta* operator()(machine_meta*const* metas, int C, machine_meta* result)
	{
	}
};
struct production_func_24 : public ifunction
{
	/* overwrite */ virtual machine_meta* operator()(machine_meta*const* metas, int C, machine_meta* result)
	{
	}
};
struct production_func_25 : public ifunction
{
	/* overwrite */ virtual machine_meta* operator()(machine_meta*const* metas, int C, machine_meta* result)
	{
	}
};
struct production_func_26 : public ifunction
{
	/* overwrite */ virtual machine_meta* operator()(machine_meta*const* metas, int C, machine_meta* result)
	{
	}
};
struct production_func_27 : public ifunction
{
	/* overwrite */ virtual machine_meta* operator()(machine_meta*const* metas, int C, machine_meta* result)
	{
	}
};
struct production_func_28 : public ifunction
{
	/* overwrite */ virtual machine_meta* operator()(machine_meta*const* metas, int C, machine_meta* result)
	{
	}
};