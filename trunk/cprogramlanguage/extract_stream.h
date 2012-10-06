// extrace_stream.h :
//

#ifndef _EXTRACE_STREAM_H_TEST_
#define _EXTRACE_STREAM_H_TEST_

#include <scerror.h>
#include <symbolmachine.h>

struct streamword
{
	std::string txt;
	int sid;
};

class extrace_stream :  public std::istream
{
	typedef std::istream _Myios;
	typedef std::streambuf _Mysb;
public:
	extrace_stream(_Mysb *_Strbuf = NULL)
	: std::istream(_Strbuf)
	{
	}

	~extrace_stream()
	{
		rdbuf(NULL);
	}
public:
	std::string whitespaces;
	compile::symbol_machine mac;
public:
	extrace_stream& operator>>(streamword& aword)
	{
		size_t Chcount = 0;
		const sentry _Ok(*this);// lock and skip whitespace

		ios_base::iostate _State = ios_base::goodbit;

		streamword tmp;
		if (_Ok)
		{	// state okay, use facet to extract
			_TRY_IO_BEGIN
			int_type _Meta = _Myios::rdbuf()->sgetc();
			for(; ; _Meta = _Myios::rdbuf()->snextc())
			{
				if (std::istream::traits_type::eq_int_type(std::istream::traits_type::eof(), _Meta))
				{	// end of file, quit
					_State |= ios_base::eofbit;
					break;
				}
				else if(whitespaces.find(_Meta) != std::string::npos)
				{// whitespace, continue
					_Myios::rdbuf()->sbumpc();
				}
				else
				{
					++Chcount;
					tmp.sid = try_to_read_symbol(tmp, _Meta);
					break;
				}
			}
			_CATCH_IO_END
		}
		aword = tmp;
		_Myios::setstate(Chcount == 0 ? _State | ios_base::failbit : _State);
		return *this;
	}
private:
	int try_to_read_symbol(streamword& aword, int_type _Meta)
	{
		mac.init();
		for(; ; _Meta = _Myios::rdbuf()->snextc())
		{
			compile::automachine::machine_meta meta(_Meta);
			if (!mac.eta(&meta))
			{
				compile::int32 csid = mac.get_cursid();
				if (csid == -1)
				{
					logerror("machine fail into an unknown status when read {%s%c}!", mac.str().c_str(), (char)_Meta);
					/*log_error*/fire("machine fail into an unknown status!");
				}
				aword.txt = mac.str();
				return csid;
			}
		}
		return -1;
	}
};

#endif