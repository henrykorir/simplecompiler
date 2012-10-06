// automachine.h : auto machine, goto next state with the given meta
//

#ifndef _AUTO_MACHINE_H_
#define _AUTO_MACHINE_H_

#include <algorithm>
#include <functional>
#include <macros.h>
#include <basic_types.h>
#include <arrayX.h>
//#include <boost/shared_ptr.hpp>
#include <shared_ptr.h>

class compiler;
NAMESPACE_BEGIN(compile)

template<typename _CmpFunc>
struct intpair_cmp : public std::binary_function<std::pair<int, int>, std::pair<int, int>, bool>
{
	bool operator()(const std::pair<int, int>& p1, const std::pair<int, int>& p2) const
	{
		return _L(p1.first, p2.first);
	}
	_CmpFunc _L;
};

typedef intpair_cmp<std::less<int> > intpair_less;
typedef intpair_cmp<std::equal_to<int> > intpair_equal;

class automachine
{
	friend class compiler;
public:
	typedef sc::tstring tstring;
	typedef std::pair<int, int> gotoitem;
	class sheetrow : public kog::smart_vector<gotoitem>
	{
		typedef kog::smart_vector<gotoitem> v;
	public:
		enum { anychar, special, exclude };
	public:
		int32 type() const { return eattype_; }
		void type(int32 t) { eattype_ = t; }
		int32 endings() const {return isendings_; }
		void endings(int32 t) { isendings_ = t; }
		void* more() const { return more_; }
		void more(void* m) { more_ = m;}
		int32 code() const { return code_; }
		void code(int32 c) { code_ = c; }
	public:
		sheetrow()
		: eattype_(special)
		, isendings_(0)
		, code_(-1)
		{}

		sheetrow(int32 etype, int32 ise, void* morei)
		: eattype_(etype)
		, isendings_(ise)
		, more_(morei)
		{
		}
	//	static sheetrow& make_any(int32 nextS, sheetrow& row);
	private:
		int32 eattype_;
		int32 isendings_;
		union 
		{
			void* more_;
			int32 code_;
		};
	};

	struct machine_meta
	{
		int32 sid;

		machine_meta(int32 meta = -1)
			: sid(meta)
		{}
	};
public:
	//typedef kog::smart_vector<gotoitem> sheetrow;
	typedef kog::smart_vector<sheetrow> sparsesheet;
	typedef kog::shared_ptr<sparsesheet> shared_sheet;
public:
	automachine();
	automachine(const shared_sheet& asheet);
	virtual ~automachine();
public:
	virtual void init();
	virtual bool eta(machine_meta* meta);
public:
	bool isaccepted() const
	{
		return sheet_->at(cstate_).endings() != 0;
	}
public:
	MEMBER_VARIABLE_GET_SET(sparsesheet, sheet, *sheet_);
	MEMBER_VARIABLE_GET_SET(int, sstate, sstate_);
//	MEMBER_VARIABLE_GET_SET(int, estate, estate_);
public:
	void swap(automachine& other)
	{
		sheet_.swap(other.sheet_);
		std::swap(cstate_, other.cstate_);
		std::swap(sstate_, other.sstate_);
//		std::swap(estate_, other.estate_);
	}
private:
	//kog::share_ptr<sparsesheet> sheet_;
	shared_sheet sheet_;
protected:
	int cstate_; // current state
	int sstate_; // start state
//	int estate_; // stop state
};

NAMESPACE_END(compile)
#endif
