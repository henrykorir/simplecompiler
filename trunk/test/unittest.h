//unittest.h:
//

#ifndef _UNITTEST_H_HUXL_
#define _UNITTEST_H_HUXL_

#include <string>

#include <macros.h>

NAMESPACE_BEGIN(sc)

class unittest
{
public:
	unittest(void);
	unittest(int min_argc, const std::string& usage);
	virtual ~unittest(void);
public:
	virtual void init(int argc, const char* argv[]) = 0;
public:
	void run(const std::string& outfolder);
	void run();
public:
	MEMBER_VARIABLE_GET_SET(std::string, out_folder, outfolder_);
private:
	virtual void run_test() = 0;
private:
	std::string outfolder_;
	std::string usage_;
	int minargc_;
};

NAMESPACE_END(sc)

#define NEW_UNITTEST(classname) \
	classname a_##classname##ut_obj

#define ASSERT_COMMAND_ARGUMENTS(minnum, usage) \
	if (argc < minnum) \
		throw std::runtime_error(std::string("too few command arguments! usage: ") + usage);

#endif
