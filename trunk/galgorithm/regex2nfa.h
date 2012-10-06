// regex2nfa.h: regex string -> nfa tinygrammar
//

#ifndef _REGEX2NFA_H_SC_
#define _REGEX2NFA_H_SC_

#include <deque>
#include <macros.h>
#include "grammaralgorithm.h"

struct proccess_content;
NAMESPACE_BEGIN(compile)
NAMESPACE_BEGIN(ga)

class regex2nfa : public grammar_algorithm
{
public:
	regex2nfa(const tstring& regexstr, tinygrammar& otput, bool is_direct)
		: grammar_algorithm("regex2nfa")
		, regexstr_(regexstr)
		, gout_(&otput)
		, is_direct_(is_direct)
	{
	}
public:
	/* overwrite */ virtual void invoke()
	{
		(*this)(regexstr_, *gout_);
	}
private:
	void operator()(const tstring& input, tinygrammar& otput);
	void tocfg(const tstring& input, tinygrammar& otput);
	void tonfa(const tinygrammar& input, tinygrammar& otput);
	void todfa_direct(const tstring& input, tinygrammar& otput);
	void init_terminates();
private:
	tstring regexstr_;
	tinygrammar* gout_;
	bool is_direct_;
private:
	int32 anychar_sid(int32 R);
	int32 digits_sid(int32 R);
	int32 word_sid(int32 R);
	int32 spaces_sid(int32 R);
	int32 new_nonterm();
	int32 make_ps(int32 L, int32 meta, int32 R = -1);
private:
	int32 make_cfg_forelem(proccess_content& pc);
private:
	std::deque<symbol> tmpsyms_;
	std::deque<production> tmpps_;
	std::list<std::string> strbuffer_;
	std::vector<int32> char2sidmap_;
	int32 eplison_;
	int32 eof_;
};

NAMESPACE_END(ga)
NAMESPACE_END(compile)
#endif
