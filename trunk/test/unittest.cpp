#include "unittest.h"
#include <iostream>
#include <list>
#include <singleton.h>
#include <typeinfo>
#include <scerror.h>

#include <typeinfo/typeinfoname.h>

using namespace sc;
using namespace kog;

#define _TRY_CATCH_ALL_

class unittest_list : public std::list<unittest*>, public kog::singleton<unittest_list>
{
public:
	int run_all_unittest(int argc, const char* argv[])
	{
		int suc = 0;
		std::clog<<"############# run all tests #############"<<std::endl;
		for(std::list<unittest*>::iterator iter = begin(); iter != end(); ++ iter)
		{
#ifdef _TRY_CATCH_ALL_
			try{
#endif
				std::clog<<typeid(**iter).name()<<std::endl;
				(*iter)->init(argc, argv);
				(*iter)->run();
				++ suc;
#ifdef _TRY_CATCH_ALL_
			}CATCH_EXCEPTIONS_COUT;
#endif
		}
		std::clog<<"############# done #############"<<std::endl;
		return suc;
	}
};

unittest::unittest(void)
: minargc_(1), usage_("")
{
	unittest_list::instance().push_back(this);
}

unittest::unittest(int min_argc, const std::string& usage)
: minargc_(min_argc), usage_(usage)
{
	unittest_list::instance().push_back(this);
}

unittest::~unittest(void)
{
}

void unittest::run(const std::string& outfolder)
{
	outfolder_ = outfolder;
	run();
}

void unittest::run()
{
//	IO::output_folder outf(outfolder_/*, false*/);
//	IO::cdswitcher outf(outfolder_);
//	std::cout<<"set output folder to: "<<outf.current_folder()<<"\n";
	run_test();
}

int main(int argc, const char* argv[])
{
	return unittest_list::instance().run_all_unittest(argc, argv);
}
