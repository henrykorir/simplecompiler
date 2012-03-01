#include "compiler.h"
#include <functional>
#include <algorithm>
#include <logger.h>
#include <fstream>
#include <scerror.h>

#include "extract.h"
#include "lalr1machine.h"

using namespace sc;
using namespace compile;
using namespace compile::doc;

compiler::compiler()
: keywords(string_2_int::max_int)
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

extern void init_grammar(tinygrammar& tig);
extern void init_machines(std::map<std::string, machine>& machines);
extern void init_syntax_machine(lalr1machine& lrm);
extern void init_keywords(kog::buckethash<std::string, int32, string_2_int>& keywords);
extern void init_separators(kog::smart_vector<sc::int32>& separators, kog::tree<int32>& sepsid);
extern void init_printablechars(kog::smart_vector<sc::int32>& printablechars);

void compiler::initialization()
{
	init_grammar(tg);
	init_printablechars(printablechars);
	init_keywords(keywords);
	init_separators(separators, sepsid);
	init_machines(machines);
	
	// 
	machines["__main__"] = machine(kog::shared_ptr<automachine>(new lalr1machine(tg)), -1);
	init_syntax_machine(*(dynamic_cast<lalr1machine*>(machines["__main__"].mac.get())));
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

	logstring("\nstart to run machine...\n");
	for(streamsplit::deqwords::const_iterator iter = words.begin(); iter != words.end(); ++ iter)
	{
		switch (iter->wordClass)
		{
		case 0: // separators
			{
				split_separators splits(iter->txt.c_str());
				for (int32 x = splits.get(); x != -1; x = splits.next())
				{
					logstring("%s\t%d\n", splits.buf.c_str(), x);
					if(!lrm.eta(x))
						fire("not expected sep!\n");
				}
			}
			break;
		case -1: // eof
			{
				logstring("%s\t%d\n", iter->txt.c_str(), -1);
				if(!lrm.eta(-1))
					fire("not expected eof!\n");
			}
			break;
		default:
			{
				int32 x = is_keywords(iter->txt);
				if(x == -1) x = iter->wordClass;
				logstring("%s\t%d\n", iter->txt.c_str(), x);
				if(!lrm.eta(x))
					fire("not expected word!\n");
			}
		}		
	}

	if(lrm.isaccepted())
		logstring("\naccepted!\n");
	else logstring("\nerror, not accepted!\n");
}