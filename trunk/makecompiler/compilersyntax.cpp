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
	production p(symbols(), L, R.get(), R.size());
	productions_.push_back(p);
	pinfos_.push_back(pinfo);
	logstring("entry production [%d] %s", pinfos_.size() - 1, p.to_string().c_str());
	return (int32)productions_.size();
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
	return make_terminate(sname, regexstring, stype, false);
}

int32 compiler_grammar::make_terminate(const tstring& sname, const tstring& regexstring, const tstring& stype, bool is_direct)
{
	int x = index(sname);
	if (x != -1) fire("found exist symbol(%s), index is %d", sname.c_str(), x);
	symbol news = make_symbol(sname, (int32)symbols_.size(), true);
	symbols_.push_back(news);
	nameIndexer_[sname] = news.sid;
	//make_index();
	// make regex string
	smacs_.push_back(kog::make_triple(regexstring, news.sid, is_direct));
	logstring("entry terminate symbol [%d] %s", news.sid, sname.c_str());
	return (int32)symbols_.size();
}

int32 compiler_grammar::make_new_keywords(const tstring& keyword)
{
	return make_terminate(keyword, keyword, "", true);
}

int32 compiler_grammar::make_new_operators(const tstring& oprs, const tstring& name)
{
	return make_terminate(oprs, oprs, "", true);
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

symbol compiler_grammar::make_symbol(const tstring& sname, int32 sid, bool isTerm)
{
	std::auto_ptr<tchar> buffer( alloc_.allocate(sname.size() + 1));
	memcpy(buffer.get(), sname.c_str(), sizeof(tchar) * (sname.size() + 1));
	symbol s;
	s.funcs = NULL;
	s.ist = isTerm ? 1 : 0;
	s.Lname = sid;
	s.sid = (int32)symbols_.size();
	s.Lfuncs = 0;
	s.name = buffer.release();

	return s;
}
