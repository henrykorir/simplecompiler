#include "compilersyntax.h"
#include <memory>
#include <logger.h>

using namespace compile;
using namespace compile::syntax;

compiler_grammar::compiler_grammar()
{
}

compiler_grammar::~compiler_grammar()
{
}

int32 compiler_grammar::make_new_production(const tstring& left, const std::vector<tstring>& rights, const prodinfo_t& pinfo)
{
	int32 L = index(left);
	kog::smart_vector<int32> R(rights.size());
	int32 i = 0;
	foreach (const tstring& r, rights.begin(), rights.end())
	{
		R[i++] = index(r);
	}

	if (L == -1 || std::find(R.begin(), R.end(), -1) != R.end())
		fire("invalidate production, some symbol not found!");
	// check has same production
	foreach (const production& tmp, productions_.begin(), productions_.end())
	{
		if (L == tmp.left() && tmp.right_size() == rights.size() &&
				std::equal(R.begin(), R.end(), tmp.right().begin()))
			fire("existing same production!");
	}

	production p(symbols(), L, R.get(), R.size());
	productions_.push_back(p);
	pinfos_.push_back(pinfo);
	logstring("entry production [%d] %s", pinfos_.size() - 1, p.to_string().c_str());
	return (int32)productions_.size();
}

int32 compiler_grammar::make_new_function(const tstring& funcname, const funcinfo_t& info)
{
	return 0;
}

int32 compiler_grammar::make_new_nonterm_symbol(const tstring& sname, const tstring& stype)
{
	int x = index(sname);
	if (x != -1) fire("found exist symbol(%s), index is %d", sname.c_str(), x);
	int32 newsid = (int32)symbols_.size();
	symbols_.push_back(make_symbol(sname, newsid, false));
	nameIndexer_[sname] = newsid;
	logstring("entry non-term symbol [%d] %s", newsid, sname.c_str());
	return (int32)symbols_.size();
}

int32 compiler_grammar::make_new_term_symbol(const tstring& sname, const tstring& regexstring, const tstring& stype)
{
	//logstring("terminate ");
	return make_terminate(sname, regexstring, stype, 1);
}

int32 compiler_grammar::make_terminate(const tstring& sname, const tstring& content, const tstring& stype, int32 flag)
{
	int x = index(sname);
	if (x != -1) fire("found exist symbol(%s), index is %d", sname.c_str(), x);
	symbol news = make_symbol(sname, (int32)symbols_.size(), true);
	symbols_.push_back(news);
	nameIndexer_[sname] = news.sid;
	if (flag > 0) // is content is regex string?
	{
		// make regex string
		smacs_.push_back(kog::make_triple(content, news.sid, flag > 1));
	}
	logstring("entry terminate symbol [%d] %s", news.sid, sname.c_str());
	return news.sid;
}

int32 compiler_grammar::make_new_keywords(const tstring& keyword, int32 basic_sid)
{
	logstring("keyword %s %d", keyword.c_str(), basic_sid);
	int32 keyword_sid = make_terminate(keyword, keyword, "", -basic_sid);

	complex_symbol_t xtmp;
	xtmp.new_sid = keyword_sid;
	xtmp.basic_sid = basic_sid;
	xtmp.flag = complex_symbol_t::keyword;
	xtmp.content = keyword;
	complexsyms_.push_back(xtmp);

	keywords_.push_back(std::make_pair(keyword, keyword_sid));
	return keyword_sid;
}

int32 compiler_grammar::make_new_operators(const tstring& oprs, const tstring& name)
{
	logstring("operator %s %s", name.c_str(), oprs.c_str());
	return make_terminate(oprs, oprs, "", 2);
}

int32 compiler_grammar::make_new_delimiters(const tstring& delimiters)
{
	logstring("entry new seperators (%s)", delimiters.c_str());
	seperators_.insert(seperators_.end(), delimiters.begin(), delimiters.end());
	std::sort(seperators_.begin(), seperators_.end());
	seperators_.erase(std::unique(seperators_.begin(), seperators_.end()),
		seperators_.end());
	return 0;
}

int32 compiler_grammar::make_new_complex_symbol(const tstring& sname, int32 sbasicid, const tstring& sfunc)
{
	logstring("complex symbol %s %d %s", sname.c_str(), sbasicid, sfunc.c_str());

	int32 new_sid = make_terminate(sname, sfunc, "", -sbasicid);

	complex_symbol_t xtmp;
	xtmp.new_sid = new_sid;
	xtmp.basic_sid = sbasicid;
	xtmp.flag = complex_symbol_t::word;
	xtmp.content = sfunc;
	complexsyms_.push_back(xtmp);

	return new_sid;
}

symbol compiler_grammar::make_symbol(const tstring& sname, int32 sid, bool isTerm)
{
	std::auto_ptr<tchar> buffer(alloc_.allocate(sname.size() + 1));
	memcpy(buffer.get(), sname.c_str(), sizeof(tchar) * (sname.size() + 1));
	symbol s;
	s.funcs = NULL;
	s.ist = isTerm ? 1 : 0;
	s.sid = sid;
	s.Lname = (int32)sname.size();
	s.Lfuncs = 0;
	s.name = buffer.release();

	return s;
}
