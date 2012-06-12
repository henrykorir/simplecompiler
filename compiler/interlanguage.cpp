#include "interlanguage.h"
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
using namespace compile::runtime;

interlanguage::interlanguage()
: main_scope_(new scope())
{
    current_scope_ = main_scope_.get();
}


interlanguage::~interlanguage()
{ 
}

scope* interlanguage::new_scope()
{
    scope* psc = new scope(current_scope_);
    return current_scope_ = psc;
}

scope* interlanguage::into_scope(runtime::scope* s)
{
    current_scope_ = s;
    return s;
}

scope* interlanguage::otof_scope()
{
    if (current_scope_ == NULL)
    {
        fire("root scope!");
    }
    return current_scope_ = current_scope_->parent();
}

scope* interlanguage::current_scope() const
{
    return current_scope_;
}

void interlanguage::generate(const std::string& finput_name, const std::string& fotput_name)
{
//    streamsplit wordsplit;
//    std::ifstream cifs(finput_name.c_str());
//    if(!cifs.is_open()) fire("can't open file: " + finput_name);
//	const streamsplit::deqwords& words = wordsplit(cifs);
//	cifs.close();
//	
//	lrmachine& lrm = *(dynamic_cast<lrmachine*>(&compiler::get_machine("__main__")));
//	lrm.init();
//
//	logstring("\nstart to run machine...\n");
//	for(streamsplit::deqwords::const_iterator iter = words.begin(); iter != words.end(); ++ iter)
//	{
//		switch (iter->wordClass)
//		{
//		case 0: // separators
//			{
//				split_separators splits(iter->txt.c_str());
//				for (int32 x = splits.get(); x != -1; x = splits.next())
//				{
//					logstring("%s\t%d\n", splits.buf.c_str(), x);
//                    automachine::machine_meta* tmp_meta = lrm.new_meta(x);
//                    lalr1machine::lalr1meta* pm = (lalr1machine::lalr1meta*)tmp_meta;
//                    pm->content = &(*iter);
//                    pm->ctype = typesystem::instance().word_type();
//					if(!lrm.eta(tmp_meta))
//						fire("not expected sep!\n");
//				}
//			}
//			break;
//		case -1: // eof
//			{
//				logstring("%s\t%d\n", iter->txt.c_str(), -1);
//                automachine::machine_meta* tmp_meta = lrm.new_meta(-1);
//                lalr1machine::lalr1meta* pm = (lalr1machine::lalr1meta*)tmp_meta;
//                pm->content = &(*iter);
//                pm->ctype = typesystem::instance().word_type();
//				if(!lrm.eta(tmp_meta))
//					fire("not expected eof!\n");
//			}
//			break;
//		default:
//			{
//				int32 x = is_keywords(iter->txt);
//				if(x == -1) x = iter->wordClass;
//				logstring("%s\t%d\n", iter->txt.c_str(), x);
//                automachine::machine_meta* tmp_meta = lrm.new_meta(x);
//                lalr1machine::lalr1meta* pm = (lalr1machine::lalr1meta*)tmp_meta;
//                pm->content = &(*iter);
//                pm->ctype = typesystem::instance().word_type();
//				if(!lrm.eta(tmp_meta))
//					fire("not expected word!\n");
//			}
//		}		
//	}
//
//	if(lrm.isaccepted())
//		logstring("\naccepted!\n");
//	else logstring("\nerror, not accepted!\n");
}
