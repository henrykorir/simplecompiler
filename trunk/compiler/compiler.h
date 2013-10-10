// compiler.h: compiler global value
//

#ifndef _COMPILER_H_SC_
#define _COMPILER_H_SC_

#include <map>
#include <list>
#include <singleton.h>
#include <stringX.h>
#include <arrayX.h>
#include <shared_ptr.h>
#include <buckethash.h>
#include <tree.h>
#include <basic_types.h>
#include <automachine.h>
#include <statemachine.h>
#include <symbolmachine.h>

#include "interlanguage.h"
#include "lalr1machine.h"

NAMESPACE_BEGIN(compile)
class compiler_setup;
NAMESPACE_BEGIN(doc)

struct machine
{
	machine()
	: sid(-2)
	{}

	machine(kog::shared_ptr<compile::automachine> m, int32 id)
		: mac(m), sid(id)
	{}

	kog::shared_ptr<compile::automachine> mac;
	int32 sid;
};

NAMESPACE_END(doc)
NAMESPACE_END(compile)

struct string_2_int
{
	enum { max_int = 100 };
	compile::int32 operator()(const std::string& s) const
	{
		const char* p = s.c_str();
		compile::int32 v = 0;
		while(*p) v += *(p ++);
		return v % max_int;
	}
};

class compiler : public kog::singleton<compiler>
{
	friend struct split_separators;
	friend class kog::singleton<compiler>;
private:
	compiler();
    ~compiler();
public:
	typedef sc::tstring tstring;
public:
	void setup(kog::shared_ptr<compile::compiler_setup> setup_obj);
	void init(const tstring& src_file, const tstring& obj_folder = "");
public:
	void prebuild();
	// build 
	void build_src2iml();
	void build_iml2asm();
public:
	compile::interlanguage* get_iml();
private:
	std::map<std::string, compile::doc::machine> machines_;
private:
	std::string _src_file;
	std::string _il_file;
	std::string _asm_file;
	kog::shared_ptr<compile::compiler_setup> setup_;
    kog::shared_ptr<compile::interlanguage> iml_;
};

#endif
