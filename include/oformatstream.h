// oformatstream.h : auto format line
//

#ifndef _OFORMATSTREAM_H_KOG_
#define _OFORMATSTREAM_H_KOG_

#include <list>
//#include <vector>
#include <string>
#include <sstream>
#include <iomanip>

#include <macros.h>
#include "indentstream.h"

NAMESPACE_BEGIN(kog);

template<typename _charT, typename _traits = std::char_traits<_charT> >
class basic_oformatstream /*: public basic_oformatstream<_charT, _traits>*/
{
public:
    typedef basic_oformatstream<_charT, _traits> _Myt;
	typedef std::basic_ostream<_charT, _traits> _Base;
	typedef kog::basic_oindentstream<_charT, _traits> _OIndent_stream;
	typedef std::basic_ios<_charT, _traits> _Myios;
	typedef std::basic_ostringstream<_charT, _traits> _OString_stream;

	typedef std::list<std::basic_string<_charT, _traits> > _Format_row;
	typedef std::list<_Format_row> _Buffer;
public:
    basic_oformatstream(_Base& os)
        : inner_os_(os)
		, tabinspaces_(4)
    {
    }

	virtual ~basic_oformatstream()
	{
		flush();
	}
public:
	_Myt& flush()
	{
		std::vector<size_t> colum_width;
		_calc_colum_width(colum_width);
		for (typename _Buffer::const_iterator iter = buffers_.begin(); iter != buffers_.end(); ++ iter)
		{
			const _Format_row& row = *iter;
			typename _Format_row::const_iterator iter_c = row.begin();
			inner_os_.newline();
			for (size_t i = 0; i < row.size(); ++ i, ++ iter_c)
			{
				inner_os_<<std::setw(colum_width[i])<<(*iter_c);
			}
		}

		buffers_.clear();

		return *this;
	}
public:
	_Myt& operator<<(_Myt& (*_Pfn)(_Myt&))
	{ // call basic_oformatstream manipulator
		return ((*_Pfn)(*this));
	}

	_Myt& operator<<(_Base& (*_Pfn)(_Base&))
	{	// call basic_ios manipulator
		(*_Pfn)((_Base &)inner_os_);
		return (*this);
	}

//	_Myt& operator<<(_Myios& (*_Pfn)(_Myios&))
//	{	// call basic_ios manipulator
//		(*_Pfn)(*(_Myios *)&inner_os_);
//		return (*this);
//	}

//	_Myt& operator<<(std::ios_base& (*_Pfn)(std::ios_base&))
//	{	// call ios_base manipulator
//		(*_Pfn)(*(std::ios_base *)&inner_os_);
//		return (*this);
//	}
public:
	/* overwrite */ virtual _Myt& fmtline()
	{
		buffers_.resize(buffers_.size() + 1);
		return *this;
	}

	template <typename _T> _Myt& operator<< (const _T& _v)
	{
		if (buffers_.empty())
		{
			inner_os_<<_v;
		}
		else
		{
			_OString_stream _oss;
			_oss<<_v;
			buffers_.back().push_back(_oss.str());
		}
		return *this;
	}
protected:
	void _calc_colum_width(std::vector<size_t>& colum_width)
	{
		for (typename _Buffer::const_iterator iter = buffers_.begin(); iter != buffers_.end(); ++ iter)
		{
			const _Format_row& row = *iter;
			if (row.size() > colum_width.size())
			{
				colum_width.resize(row.size(), 0);
			}
			typename _Format_row::const_iterator iter_c = row.begin();
			for (size_t i = 0; i < row.size(); ++ i, ++ iter_c)
			{
				colum_width[i] = std::max(colum_width[i], ((*iter_c).size() / tabinspaces_ + 1) * tabinspaces_);
			}
		}
	}
protected:
	size_t tabinspaces_;
private:
	_Buffer buffers_;
	_OIndent_stream inner_os_;
};

typedef basic_oformatstream<char> oformatstream;
typedef basic_oformatstream<wchar_t> owformatstream;

template<typename _charT, typename _traits> basic_oformatstream<_charT, _traits>& /*newline*/fmtline(basic_oformatstream<_charT, _traits>& os)
{
	return os.fmtline();
}

template<typename _charT, typename _traits> basic_oformatstream<_charT, _traits>& flush(basic_oformatstream<_charT, _traits>& os)
{
	return os.flush();
}

NAMESPACE_END(kog);
#endif
