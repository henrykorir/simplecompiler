// indent.h : auto indent
//

#ifndef _INDENT_H_KOG_
#define _INDENT_H_KOG_

#include <macros.h>
#include <ostream>

NAMESPACE_BEGIN(kog);


template<typename _charT, typename _traits = std::char_traits<_charT> >
class basic_oindentstream : public std::basic_ostream<_charT, _traits>
{
public:
    typedef basic_oindentstream<_charT, _traits> _Myt;
    typedef std::basic_ostream<_charT, _traits> _Base;
public:
    basic_oindentstream(_Base& os)
        : _Base((std::basic_streambuf<_charT, _traits>*)NULL)
		, tabindent_(0)
    {
        rdbuf(os.rdbuf());
    }

	~basic_oindentstream()
	{
		rdbuf((std::basic_streambuf<_charT, _traits>*)NULL);
	}
public:
    int indent() const
    {
        return tabindent_;
    }

    int indent(int tabs)
    {
        tabs = std::max(0, tabs);
        std::swap(tabs, tabindent_);
        return tabs;
    }

private:
    int tabindent_;
};

template<typename _charT, typename _traits> std::basic_ostream<_charT, _traits>& inctab(std::basic_ostream<_charT, _traits>& os)
{
    basic_oindentstream<_charT, _traits>& indent_os = dynamic_cast<basic_oindentstream<_charT, _traits>&>(os);
    indent_os.indent(indent_os.indent() + 1);
    return os;
}

template<typename _charT, typename _traits> std::basic_ostream<_charT, _traits>& dectab(std::basic_ostream<_charT, _traits>& os)
{
    basic_oindentstream<_charT, _traits>& indent_os = dynamic_cast<basic_oindentstream<_charT, _traits>&>(os);
    indent_os.indent(indent_os.indent() - 1);
    return os;
}

template<typename _charT, typename _traits> std::basic_ostream<_charT, _traits>& newline(std::basic_ostream<_charT, _traits>& os)
{
    basic_oindentstream<_charT, _traits>& indent_os = dynamic_cast<basic_oindentstream<_charT, _traits>&>(os);
    indent_os<<std::endl;
    for (int i = 0; i < indent_os.indent(); ++ i)
    {
        indent_os<<(_charT)('\t');
    }
    return os;
}

typedef basic_oindentstream<char> oindentstream;
typedef basic_oindentstream<wchar_t> oindentwstream;

NAMESPACE_END(kog);
#endif
