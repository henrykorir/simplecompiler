// extrace_stream.h :
//

#ifndef _EXTRACE_STREAM_H_TEST_
#define _EXTRACE_STREAM_H_TEST_

#include <logger.h>
#include <scerror.h>
#include <symbolmachine.h>

struct streamword
{
	std::string txt;
	int sid;
};

class extrace_stream
{
public:
	//typedef compile::tistream::_Mysb _Mysb;
	typedef std::basic_streambuf<compile::tchar> _Mysb;
	typedef compile::tistream::int_type int_type;
public:
	extrace_stream(compile::tistream& is, 
		const compile::symbol_machine& m, 
		int eof_sid = -1,
		const compile::tstring& whitespaces = "",
		bool skipspaceword = true)
		: input_(&is)
		, mac_(m)
		, eof_(eof_sid)
		, whitespaces_(whitespaces)
		, skipspace_(skipspaceword)
	    , hasgeneof_(0)
	{
	}

	virtual ~extrace_stream()
	{
	}
public:
	extrace_stream& operator>>(streamword& aword)
	{
		size_t Chcount = 0;
		const compile::tistream::sentry _Ok(*input_);// lock and skip whitespace

		std::ios_base::iostate _State = std::ios_base::goodbit;

		_Mysb* rdbuf = input_->rdbuf();
		streamword tmp;
		tmp.sid = eof_;
		if (_Ok)
		{	// state okay, use facet to extract
			try {
				int_type _Meta = rdbuf->sgetc();
				for(; ; _Meta = rdbuf->snextc())
				{
					if (std::istream::traits_type::eq_int_type(compile::tistream::traits_type::eof(), _Meta))
					{	// end of file, quit
						append_eof_word(tmp);
						_State |= std::ios_base::eofbit;
						break;
					}
					else if(whitespaces_.find(_Meta) != compile::tstring::npos)
					{// whitespace, continue
						rdbuf->sbumpc();
					}
					else
					{
						++ Chcount;
						tmp.sid = try_to_read_symbol(tmp, _Meta);
						break;
					}
				}
			}catch(...){
				append_eof_word(tmp);
				_State |= std::ios_base::failbit;
			}
		}
		else {
			append_eof_word(tmp);
		}
		aword = tmp;
		input_->setstate(Chcount == 0 ? _State | std::ios_base::failbit : _State);
		return *this;
	}
public:
	operator bool() const
	{
		return !eof();
	}

	bool eof() const
	{
		return hasgeneof_ >= 2;
	}
private:
	int try_to_read_symbol(streamword& aword, int_type _Meta)
	{
		mac_.init();
		_Mysb* rdbuf = input_->rdbuf();
		compile::automachine::machine_meta meta(_Meta);
		for(; ; _Meta = rdbuf->snextc())
		{
			meta.sid = _Meta;
			if (!mac_.eta(&meta))
			{
				compile::int32 csid = mac_.get_cursid();
				if (csid == -1)
				{
					logerror("machine fail into an unknown status when read {%s%c}!", mac_.str().c_str(), (char)_Meta);
					/*log_error*/fire("machine fail into an unknown status!");
				}
				aword.txt.swap(mac_.str());
				return csid;
			}
		}
		return -1;
	}
	
	void append_eof_word(streamword& tmp)
	{
		if (hasgeneof_)
		{
			tmp.sid = -1;
			hasgeneof_ = 2;
		}
		else
		{
			tmp.txt = "#";
			tmp.sid = eof_;
			hasgeneof_ = 1;
		}
	}
private:
	compile::tistream* input_;
	compile::symbol_machine mac_;
	compile::tstring whitespaces_;
	bool skipspace_;
	int eof_;
	int hasgeneof_;
};

#endif
