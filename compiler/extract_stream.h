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

	size_t line;
	size_t col;
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
		, line_(1)
		, col_(0)
	{
	}

	virtual ~extrace_stream()
	{
	}
public:
	size_t get_line_count() const { return line_; }
	size_t get_col_count() const { return col_; }
public:
	extrace_stream& operator>>(streamword& aword)
	{
		size_t Chcount = 0;
		const compile::tistream::sentry _Ok(*input_, /*!skipspace_*/true);// lock and skip whitespace

		std::ios_base::iostate _State = std::ios_base::goodbit;

		rdbuf_ = input_->rdbuf();
		streamword tmp;
		tmp.sid = eof_;
		if (_Ok)
		{	// state okay, use facet to extract
			try {
				int_type _Meta = rdbuf_->sgetc();
				logdebug("read char (%c)(%d)", (char)_Meta, _Meta);
				for(; ; _Meta = _snextc())
				{
					if (std::istream::traits_type::eq_int_type(compile::tistream::traits_type::eof(), _Meta))
					{	// end of file, quit
						//++ Chcount;
						append_eof_word(tmp);
						_State |= std::ios_base::eofbit;
						break;
					}
					else if(whitespaces_.find(_Meta) != compile::tstring::npos)
					{// whitespace, continue
						logstring("enter whitespaces");
						if (skipspace_) /*rdbuf->sbumpc()*/continue;
						else
						{
							// generate space word
							tmp.sid = try_to_read_space(tmp, _Meta);
							break;
						}
					}
					else
					{
						++ Chcount;
						tmp.sid = try_to_read_symbol(tmp, _Meta);
						break;
					}
				}
			}catch(...){
				logwarning("read failed!");
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
	// read spaces
	int try_to_read_space(streamword& aword, int_type _Meta)
	{
		compile::tstring buffer;
		for(int_type meta = _Meta; meta == _Meta; _Meta = _snextc())
		{
			buffer.push_back(_Meta);
		}
		//buffer.append('\0');
		aword.txt = buffer.c_str();
		return -2;
	}

	// read symbol
	int try_to_read_symbol(streamword& aword, int_type _Meta)
	{
		mac_.init();
		compile::automachine::machine_meta meta(_Meta);
		aword.line = line_;
		aword.col = col_;
		for(; ; _Meta = _snextc())
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
	int_type _snextc()
	{
		int_type _Last_meta = rdbuf_->sgetc();
		int_type _Meta = rdbuf_->snextc();
		logdebug("read char (%c)(%d)", (char)_Meta, _Meta);
		if ((_Meta == '\n' && _Last_meta != '\r') || _Meta == '\r')
		{
			++ line_;
			logdebug("new line %d", (int)line_);
			col_ = 0;
		}
		else
		{
			++ col_;
		}
		return _Meta;
	}
private:
	compile::tistream* input_;
	_Mysb* rdbuf_;
	compile::symbol_machine mac_;
	compile::tstring whitespaces_;
	bool skipspace_;
	int eof_;
	int hasgeneof_;

	size_t line_;
	size_t col_;
};

#endif
