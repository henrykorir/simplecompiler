#include "compiler.h"
#include <functional>
#include <algorithm>
#include <logger.h>
#include <fstream>
#include <scerror.h>
#include <typeinfo>

#include "extract.h"
#include "lalr1machine.h"

using namespace sc;
using namespace compile;
using namespace compile::doc;

compiler::compiler()
: keywords(string_2_int::max_int)
, iml_(new interlanguage())
{
}

compiler::~compiler()
{
}

state_machine compiler::get_number_machine()
{
	return *(dynamic_cast<state_machine*>(instance().machines["number"].mac.get()));
}

state_machine compiler::get_symbol_machine()
{
	return *(dynamic_cast<state_machine*>(instance().machines["symbol"].mac.get()));
}

state_machine compiler::get_string_machine()
{
	return *(dynamic_cast<state_machine*>(instance().machines["string"].mac.get()));
}

bool compiler::is_separator(int32 elem)
{
	return std::find(instance().separators.begin(), 
			instance().separators.end(), elem) != instance().separators.end();
}

int compiler::get_all_machines(std::list<machine>& mlist)
{
	for(std::map<std::string, machine>::iterator iter = instance().machines.begin(); iter != instance().machines.end(); ++ iter)
		if (typeid(*iter->second.mac) == typeid(state_machine))
		{
			mlist.push_back(iter->second);
		}
		
	return (int32)mlist.size();
}
    
automachine& compiler::get_machine(const std::string& machine_name)
{
    std::map<std::string, compile::doc::machine>& ms = instance().machines;
    std::map<std::string, compile::doc::machine>::iterator iter = ms.find(machine_name);
    if (iter == ms.end())
        fire("not found machine, " + machine_name);
    return *(iter->second.mac);
}
    
compile::interlanguage& compiler::getiml()
{
    return *(instance().iml_);
}

extern void init_grammar(tinygrammar& tig);
extern void init_machines(std::map<std::string, machine>& machines);
extern void init_syntax_machine(lalr1machine& lrm);
extern void init_keywords(kog::buckethash<std::string, int32, string_2_int>& keywords);
extern void init_separators(kog::smart_vector<sc::int32>& separators, kog::tree<int32>& sepsid);
extern void init_printablechars(kog::smart_vector<sc::int32>& printablechars);
extern void init_production_functions(kog::smart_vector<ifunction*>& pfuncs);

void compiler::initialization()
{
	init_grammar(tg);
	init_printablechars(printablechars);
	init_keywords(keywords);
	init_separators(separators, sepsid);
	init_machines(machines);

    kog::smart_vector<ifunction*> tmp_prods;
    init_production_functions(tmp_prods);
    
    // create new machine
    lalr1machine* lalr1mac = new lalr1machine(tg, tmp_prods);
	machines["__main__"] = machine(kog::shared_ptr<automachine>(lalr1mac), -1);
	init_syntax_machine(*lalr1mac);
}


int32 compiler::is_keywords(const std::string& s) const
{
	const int32* x = keywords.find(s);
	if (!x) return -1;
	return *x;
}

struct split_separators
{
	split_separators(const tchar* w)
		: aword(w)
		, x(-1)
	{
		next();
	}

	int32 get() const
	{
		return x;
	}

	int32 next()
	{
		kog::tree<int32>::link p = compiler::instance().sepsid.root();
		buf.clear();
		while(*aword && p->nc)
		{
			unsigned char t = *(const unsigned char*)aword;
			kog::tree<int32>::link q = p->children[t];
			if(q) { buf.push_back(*(aword ++)); p = q; }
			else break;
		}

		return x = p->v;
	}

	const tchar* aword;
	tstring buf;
	int32 x;
};

void compiler::check(const std::string& fname)
{
	streamsplit wordsplit;
	std::ifstream cifs(fname.c_str());
	if(!cifs.is_open()) fire("can't open file: " + fname);
	const streamsplit::deqwords& words = wordsplit(cifs);
	cifs.close();
	
	lrmachine& lrm = *(dynamic_cast<lrmachine*>(machines["__main__"].mac.get()));
	lrm.init();

	logstring("\nstart to run machine...");
	for(streamsplit::deqwords::const_iterator iter = words.begin(); iter != words.end(); ++ iter)
	{
		switch (iter->wordClass)
		{
		case 0: // separators
			{
				split_separators splits(iter->txt.c_str());
				for (int32 x = splits.get(); x != -1; x = splits.next())
				{
					logstring("%s\t%d", splits.buf.c_str(), x);
                    automachine::machine_meta* tmp_meta = lrm.new_meta(x);
                    lalr1machine::lalr1meta* pm = (lalr1machine::lalr1meta*)tmp_meta;
                    pm->content = &(*iter);
                    pm->ctype = typesystem::instance().word_type();
					if(!lrm.eta(tmp_meta))
						fire("not expected sep!\n");
				}
			}
			break;
		case -1: // eof
			{
				logstring("%s\t%d\n", iter->txt.c_str(), -1);
                automachine::machine_meta* tmp_meta = lrm.new_meta(-1);
                lalr1machine::lalr1meta* pm = (lalr1machine::lalr1meta*)tmp_meta;
                pm->content = &(*iter);
                pm->ctype = typesystem::instance().word_type();
				if(!lrm.eta(tmp_meta))
					fire("not expected eof!\n");
			}
			break;
		default:
			{
				int32 x = is_keywords(iter->txt);
				if(x == -1) x = iter->wordClass;
				logstring("%s\t%d", iter->txt.c_str(), x);
                automachine::machine_meta* tmp_meta = lrm.new_meta(x);
                lalr1machine::lalr1meta* pm = (lalr1machine::lalr1meta*)tmp_meta;
                pm->content = &(*iter);
                pm->ctype = typesystem::instance().word_type();
				if(!lrm.eta(tmp_meta))
					fire("not expected word!\n");
			}
		}		
	}

	if(lrm.isaccepted())
		logstring("\naccepted!");
	else logstring("\nerror, not accepted!");
}
