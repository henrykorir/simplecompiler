#include "extract.h"
#include <scerror.h>
#include <logger.h>

#include <iostream>
#include "statemachine.h"
#include "compiler.h"

using namespace sc;
using namespace compile;
using namespace compile::doc;

#ifndef _TRY_IO_BEGIN 
#define _TRY_IO_BEGIN try{
#endif

#ifndef _CATCH_IO_END
#define _CATCH_IO_END }catch(...){\
	_State |= ios_base::failbit;}
#endif

class iwordstream : public std::istream
{
	typedef std::istream _Myios;
	typedef std::streambuf _Mysb;
public:
	iwordstream(_Mysb *_Strbuf = NULL)
	: std::istream(_Strbuf)
	{
	}
public:
	iwordstream& operator>>(word& aword)
	{
		size_t Chcount = 0;
		const sentry _Ok(*this);// lock and skip whitespace

		ios_base::iostate _State = ios_base::goodbit;

		word tmp;
		if (_Ok)
		{	// state okay, use facet to extract
		//	const _Ctype& _Ctype_fac = std::use_facet<_Ctype>(ios_base::getloc());

			_TRY_IO_BEGIN
			int_type _Meta = _Myios::rdbuf()->sgetc();
			for(; ; _Meta = _Myios::rdbuf()->snextc())
			{
				if (std::istream::traits_type::eq_int_type(std::istream::traits_type::eof(), _Meta))
				{	// end of file, quit
					_State |= ios_base::eofbit;
					break;
				}
				//else if(_Ctype_fac.is(_Ctype::space,
				//			std::istream::traits_type::to_char_type(_Meta)))
				else if(_Meta == ' ' || _Meta == '\t' || _Meta == '\r' || _Meta == '\n')
				{// whitespace, continue
					//_Myios::rdbuf()->sbumpc();
				}
				else
				{
					++Chcount;
					tmp.wordClass = try_to_read_symbol(tmp, _Meta);
					//tmp.txt.push_back(std::istream::char_type(0));	// add terminating null character
					break;
				}
			}
			_CATCH_IO_END
		}
		aword.swap(tmp);
		_Myios::setstate(Chcount == 0 ? _State | ios_base::failbit : _State);
		return *this;
	}
private:
	int try_to_read_symbol(word& aword, int_type _Meta)
	{
		if(int mtype = match_machines(aword, _Meta))
		{
			return mtype;
		}
		else if(compiler::is_separator(_Meta)) // separators
		{
			do{
				aword.txt.push_back(_Meta);
				_Meta = _Myios::rdbuf()->snextc();
			}while(compiler::is_separator(_Meta));
			
			return 0;
		}
		else
		{	
			//_State |= ios_base::badbit;
			throw std::runtime_error("invalidate charactor!");
		}
		return -1;
	}

	bool read_content(_Str& txt, state_machine& nm)
	{
		nm.init();
		int_type _Meta = _Myios::rdbuf()->sgetc();
		for(; ; _Meta = _Myios::rdbuf()->snextc())
		{
			bool is_ending = nm.isaccepted();
			if(!nm.eta(_Meta)) 
			{
				if(is_ending)
				{
					break;
				}
				else
				{
					// putback all read charactors
					for(size_t i = txt.size(); i; -- i)
					{
						_Myios::rdbuf()->sputbackc(txt[i-1]);
					}
					return false;
				}
			}
			else
			{
				txt.push_back(_Meta);
			}
		}
		txt = nm.str();
		return true;
	}

	int32 match_machines(word& aword, int_type _Meta)
	{
		std::list<machine> mlist;
		compiler::get_all_machines(mlist);
		typedef std::list<machine>::iterator machine_iterator;
		for(machine_iterator iter = mlist.begin(); iter != mlist.end();)
		{
			state_machine& m = *dynamic_cast<state_machine*>(iter->mac.get());
			m.init();
			if(!m.eta(_Meta)) iter = mlist.erase(iter);
			else  { ++ iter; }
		}
		switch(mlist.size())
		{
		case 0: return 0;
		case 1: 
			{
				std::string buf;
				state_machine& m = *dynamic_cast<state_machine*>(mlist.begin()->mac.get());
				if(read_content(buf, m))
				{
					aword.txt.swap(buf);
					return mlist.begin()->sid;
				}
				else return 0;
			}
		default:
				fire("we can only accept one state machine");
		}
		return 0;
	}
};

compile::doc::streamsplit::streamsplit()
{
}

compile::doc::streamsplit::~streamsplit()
{
}

streamsplit::deqwords& streamsplit::operator()(std::istream& is)
{
	iwordstream iws;
	iws.rdbuf(is.rdbuf());
	word aword;
	while(iws>>aword)
	{
		words_.push_back(aword);
#ifdef DEBUG_OUTPUT
		logstring("read word: %s\n", aword.txt.c_str());
#endif
	}
	bool isdone = iws.eof();
	iws.rdbuf(NULL);
	if(!isdone) throw scerror("failed read word!");

	word eof_word;
	eof_word.txt = "#";
	eof_word.wordClass = -1;
	words_.push_back(eof_word);
	return words_;
}
