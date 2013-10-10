// ilinestream.h: read a line
//

#ifndef _ILINE_STREAM_H_KOG_
#define _ILINE_STREAM_H_KOG_

#include <fstream>
#include <string>

#include <macros.h>

NAMESPACE_BEGIN(kog);

template<typename _charT, typename _traits = std::char_traits<_charT> >
class basic_ilinestream : public std::basic_istream<_charT, _traits>
{
public:
    typedef basic_ilinestream<_charT, _traits> _Myt;
    typedef std::basic_istream<_charT, _traits> _Base;
public:
	basic_ilinestream()
	: _Base((std::basic_streambuf<_charT, _traits>*)NULL)
	, iLine_(0)
	{
	}

	basic_ilinestream(const _Base& is)
		: _Base((std::basic_streambuf<_charT, _traits>*)NULL)
		, iLine_(0)
	{
		this->rdbuf(is.rdbuf());
	}
	
	~basic_ilinestream()
	{
		rdbuf((std::basic_streambuf<_charT, _traits>*)NULL);
	}

public:
	_Myt& getline(std::basic_string<_charT, _traits>& buf)
	{
		if(std::getline(*this, buf)) ++ iLine_;
		return *this;
	}
public:
	size_t line_counter() const
	{
		return iLine_;
	}
private:
	size_t iLine_;
};

typedef basic_ilinestream<char> ilinestream;

NAMESPACE_END(kog);

#endif
