// stringX.h
//

#ifndef _STRINGX_H_HUXL_
#define _STRINGX_H_HUXL_

#include <macros.h>
#include <stringXF.h>

NAMESPACE_BEGIN(stringX)

template<typename _Elem,
	typename _Traits = std::char_traits<_Elem>,
	typename _Ax = std::allocator<_Elem> >
class xstring_basic
	: public std::basic_string<_Elem, _Traits, _Ax>
{
public:
	typedef xstring_basic<_Elem, _Traits, _Ax> _Myt;
	typedef std::basic_string<_Elem, _Traits, _Ax> _Mybase;
	//typedef typename _Mybase::_Alloc _Alloc;
	typedef typename _Mybase::size_type size_type;
	typedef typename _Mybase::difference_type _Dift;
	typedef _Dift difference_type;
	typedef typename _Mybase::pointer _Tptr;
	typedef typename _Mybase::const_pointer _Ctptr;
	typedef _Tptr pointer;
	typedef _Ctptr const_pointer;
	typedef typename _Mybase::reference _Reft;
	typedef _Reft reference;
	typedef typename _Mybase::const_reference const_reference;
	typedef typename _Mybase::value_type value_type;

	//typedef typename _Mybase::iterator iterator;

	typedef std::basic_stringstream<_Elem, _Traits, _Ax> _Stream;
	typedef std::basic_istringstream<_Elem, _Traits, _Ax> _IStream;
	typedef std::basic_ostringstream<_Elem, _Traits, _Ax> _OStream;

public:
	xstring_basic()
	{}

	xstring_basic(const _Mybase& _Right, size_t _Roff, size_t n = _Mybase::npos)
		: _Mybase(_Right, _Roff, _Mybase::npos)
	{}

	xstring_basic(const _Elem* _Ptr, size_t _Count)
		: _Mybase(_Ptr, _Count)
	{}

	xstring_basic(const _Elem* _Ptr)
		: _Mybase(_Ptr)
	{}

	xstring_basic(size_t _Count, _Elem _Ch)
		: _Mybase(_Count, _Ch)
	{}

	template<class _It>
	xstring_basic(_It _First, _It _Last)
		: _Mybase(_First, _Last)
	{}

	xstring_basic(const _Mybase& str)
		: _Mybase(str)
	{}

public:
	_Mybase& base()
	{
		return *this;
	}

	const _Mybase& base() const
	{
		return *this;
	}

	operator _Mybase&()
	{
		return *this;
	}

	operator const _Mybase&() const
	{
		return *this;
	}

public:
	enum {trim_begin, trim_end, trim_both};
	// erase the pad spaces of the string
	// param@ path, the original string
	// param@ trimType
	//		trimType == 1 : erase the ending spaces
	//		trimType == 2 : erase the leading spaces
	//		trimType == 3 : erase the leading and ending spaces
	_Myt& trim(int _TrimType = trim_both)
	{
		size_t pos = 0;
		size_t len = _Mybase::size();
		const _Elem* ptr = _Mybase::c_str();

		if(_TrimType & 0x01)
		{
			while(len && isspace(ptr[len-1])) -- len;
		}
		if(_TrimType & 0x02)
		{
			while(len && isspace(ptr[pos])) ++ pos, -- len;
		}
		return (*this = substr(pos, len));
	}

public:
	// format string
	static _Myt format(const _Elem* fmt, ...)
	{
		_Myt _Out;

		va_list argptr;
		va_start(argptr, fmt);
		strformat::format(fmt, argptr, _Out);

		va_end(argptr);
		return _Out;
	}

	static _Myt trim(const _Myt& _Str, int _TrimType = 3)
	{
		_Myt temp(_Str);
		return temp.trim(_TrimType);
	}

	static bool isdigit(_Elem _Ch)
	{
		Format::IsDigit _isDigit;
		return _isDigit(_Ch);
	}

	static bool isinteger(const _Myt& _Str)
	{
		_Myt temp = _Myt::trim(_Str);
		size_t idx = 0;
		if(temp.empty()) return false;
		else if(temp[0] == Format::meta<_Elem>('+') || temp[1] == Format::meta<_Elem>('-'))
		{
			++ idx;
		}
		while(idx <= temp.size()&& isspace(temp[idx])) ++ idx;
		if(idx == temp.size()) return false;
		while(idx <= temp.size() && isdigit(temp[idx])) ++ idx;
		return temp.size() == idx;
	}

	static bool islatter(_Elem _Ch)
	{
		unsigned short t = _Ch;
		return (t <= 'Z' && t >= 'A') || (t <= 'z' && t >= 'a');
	}

	static bool isspace(_Elem _Ch)
	{
		Format::IsSpace _isSpace;
		return _isSpace(_Ch);
	}

	template<typename _Tz>
	static _Myt tostring(const _Tz& _value)
	{
		std::basic_ostringstream<_Elem> oss;
		oss<<_value;
		return oss.str();
	}

	template<typename _Tz>
	static _Tz parse(const _Myt& str)
	{
		std::string& msg;
		_Tz _tz;
		if(!tryparse(str, _tz, msg))
			throw std::runtime_error(msg.c_str());
		return _tz;
	}

	template<typename _Tz>
	static bool tryparse(const _Myt& str, _Tz& _tz)
	{
		std::string& msg;
		return tryparse(str, _tz, msg);
	}

private:
	template<typename _Tz>
	static bool tryparse(const _Myt& str, _Tz& _tz, std::string& exceptionMsg)
	{
		std::basic_istringstream<_Elem> iss;
		iss.str(str);
		iss>>_tz;
		if(iss.fail())
		{
			//if(Format::swcvt::convert(str, buf))
			//{
			//	exceptionMsg = "can't parse string \"" + str +
			//		"\" to " + typeid(_Tz).name();
			//}else exceptionMsg = "can't parse string to " + typeid(_Tz).name();
			return false;
		}
		return true;
	}

public:
	bool getstream(_Stream& _stream)
	{
		_stream.clear();
		_stream.str(*this);
		return _stream.good();
	}

	bool getstream(_IStream& _istream)
	{
		_istream.clear();
		_istream.str(*this);
		return _istream.good();
	}

	bool getstream(_OStream& _ostream)
	{
		_ostream.clear();
		_ostream.str(*this);
		return _ostream.good();
	}

public:
	template<typename _Tz>
	_Myt& operator +=(const _Tz& _tz)
	{
		_OStream oss;
		oss<<_tz;
		return ((*this) += oss.str());
	}

	template<typename _Tz>
	_Myt operator +(const _Tz& _tz) const
	{
		std::basic_ostringstream<_Elem> oss;
		oss<<_tz;

		return (*this + oss.str());
	}
public:
	_Myt& operator +=(const _Myt& _Right)
	{
		append(_Right);
		return *this;
	}

	_Myt operator +(const _Mybase& _Right) const
	{
		_Myt temp = *this;
		return temp += _Right;
	}

	_Myt operator +(const _Elem* _Right) const
	{
		_Myt temp = *this;
		temp.append(_Right);
		return temp;
	}

	_Myt operator +(const _Elem _Right) const
	{
		_Myt temp = *this;
		temp.append(_Right);
		return temp;
	}
public:
	_Myt& to_upper()
	{
		typename _Mybase::iterator iter_end = _Mybase::end();
		unsigned short a = 'a';
		unsigned short z = 'z';
		unsigned short A = 'A';

		for(typename _Mybase::iterator iter = _Mybase::begin(); iter != iter_end; ++ iter)
		{
			if(((unsigned short)(*iter)) >= a && ((unsigned short)(*iter)) <= z)
				*iter = (_Elem)(((unsigned short)(*iter)) - a + A);
		}

		return *this;
	}

	_Myt& to_lower()
	{
		typename _Mybase::iterator iter_end = _Mybase::end();
		unsigned short a = 'a';
		unsigned short A = 'A';
		unsigned short Z = 'Z';
		for(typename _Mybase::iterator iter = _Mybase::begin(); iter != iter_end; ++ iter)
		{
			if(((unsigned short)(*iter)) >= A && ((unsigned short)(*iter)) <= Z)
				*iter = (_Elem)(((unsigned short)(*iter)) - A + a);
		}
		return *this;
	}
public:
	OVERWRITE_FUNCTION_1(operator=, const _Mybase&, _Right);
	OVERWRITE_FUNCTION_1(operator=, const _Elem*&, _Ptr);
	OVERWRITE_FUNCTION_1(operator=, _Elem, _Ch);

public:
	OVERWRITE_FUNCTION_1(operator+=, const _Mybase&, _Right);
	OVERWRITE_FUNCTION_1(operator+=, const _Elem*&, _Ptr);
	OVERWRITE_FUNCTION_1(operator+=, _Elem, _Ch);
public:

	OVERWRITE_FUNCTION_1(append, const _Mybase&, _Right);
	OVERWRITE_FUNCTION_3(append, const _Mybase&, _Right, size_type, _Roff, size_type, _Count);
	OVERWRITE_FUNCTION_2(append, const _Elem*, _Ptr, size_type, _Count);
	OVERWRITE_FUNCTION_1(append, const _Elem*, _Ptr);
	OVERWRITE_FUNCTION_2(append, size_type, _Count, _Elem, _Ch);
	template<class _It>
	OVERWRITE_FUNCTION_2(append, _It, _First, _It, _Last);

public:
	OVERWRITE_FUNCTION_1(assign, const _Mybase&, _Right);
	OVERWRITE_FUNCTION_3(assign, const _Mybase&, _Right, size_type, _Roff, size_type, _Count);
	OVERWRITE_FUNCTION_2(assign, const _Elem*, _Ptr, size_type, _Count);
	OVERWRITE_FUNCTION_1(assign, const _Elem*, _Ptr);
	OVERWRITE_FUNCTION_2(assign, size_type, _Count, _Elem, _Ch);
	template<class _It>
	OVERWRITE_FUNCTION_2(assign, _It, _First, _It, _Last);
public:
	OVERWRITE_FUNCTION_2(insert, size_type, _Off, const _Mybase&, _Right);
	OVERWRITE_FUNCTION_4(insert, size_type, _Off, const _Mybase&, _Right, size_type, _Roff, size_type, _Count);
	OVERWRITE_FUNCTION_3(insert, size_type, _Off, const _Elem*, _Ptr, size_type, _Count);
	OVERWRITE_FUNCTION_2(insert, size_type, _Off, const _Elem*, _Ptr);
	OVERWRITE_FUNCTION_3(insert, size_type, _Off, size_type, _Count, _Elem, _Ch);

public:
	_Myt& erase(size_type _Off = 0, size_type _Count = _Mybase::npos)
	{
		base().erase(_Off, _Count);
		return *this;
	}

public:
	OVERWRITE_FUNCTION_3(replace, size_type, _Off, size_type, _N0, const _Myt&, _Right);
	OVERWRITE_FUNCTION_3(replace, size_type, _Off, size_type, _N0, const _Elem*, _Ptr);
	OVERWRITE_FUNCTION_3(replace, typename _Mybase::iterator, _First, typename _Mybase::iterator, _Last, const _Myt&, _Right);
	OVERWRITE_FUNCTION_3(replace, typename _Mybase::iterator, _First, typename _Mybase::iterator, _Last, const _Elem*, _Ptr);
	OVERWRITE_FUNCTION_4(replace, size_type, _Off, size_type, _N0, const _Elem*, _Ptr, size_type, _Count);
	OVERWRITE_FUNCTION_4(replace, size_type, _Off, size_type, _N0, size_type, _Count, _Elem, _Ch);
	OVERWRITE_FUNCTION_4(replace, typename _Mybase::iterator, _First, typename _Mybase::iterator, _Last, const _Elem*, _Ptr, size_type, _Count);
	OVERWRITE_FUNCTION_4(replace, typename _Mybase::iterator, _First, typename _Mybase::iterator, _Last, size_type, _Count, _Elem, _Ch);
	template<class _It>
	OVERWRITE_FUNCTION_4(replace, typename _Mybase::iterator, _First, typename _Mybase::iterator, _Last, _It, _First2, _It, _Last2);

	_Myt& replace(size_type _Off,
		size_type _N0, const _Mybase& _Right, size_type _Roff, size_type _Count)
	{
		base().replace(_Off, _N0, _Right, _Roff, _Count);
		return *this;
	}

public:
	_Myt substr(size_type _Off = 0, size_type _Count = _Mybase::npos) const
	{
		return _Myt(base().substr(_Off, _Count));
	}
};

template<typename _Elem,
	typename _Traits = std::char_traits<_Elem>,
	typename _Ax = std::allocator<_Elem> >
class basic_stringbuffer : public std::basic_ostringstream<_Elem, _Traits, _Ax>
{
};

template<typename _Elem>
xstring_basic<_Elem> xstringcat(const _Elem* str1, const _Elem* str2)
{
	return xstring_basic<_Elem>(str1) += str2;
}

typedef xstring_basic<char> xstring;
typedef xstring_basic<wchar_t> xwstring;
typedef basic_stringbuffer<char> stringbuffer;
typedef basic_stringbuffer<wchar_t> wstringbuffer;

NAMESPACE_END(stringX)

#endif
