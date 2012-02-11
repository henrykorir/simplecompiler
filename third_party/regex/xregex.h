#pragma once

#ifndef _XREGEX_H_HUXL_
#define _XREGEX_H_HUXL_

#include <string>
#include <typeinfo>
#include <stdexcept>
#include <sstream>

namespace stringX{

class convert_error : public std::runtime_error
{
public:
	convert_error(const std::string& error_msg)
		: std::runtime_error(error_msg.c_str())
	{}

	template<typename _Tx, typename _Ty>
	convert_error(const _Tx& _Src, const _Ty& _Dst)
		: std::runtime_error(new_error_msg(_Src, _Dst))
	{
	}
private:
	template<typename _Tx, typename _Ty>
	static std::string new_error_msg(const _Tx& _Src, const _Ty& _Dst)
	{
		std::ostringstream oss;
		oss<<"["<<typeid(_Tx).name()<<"]("<<_Src<<")"
			<<" -> "
			<<"["<<typeid(_Ty).name()<<"]";
	}
};

class xregex
{
public:
	typedef char char_type;
	typedef std::basic_string<char_type> tstring;
public:
	xregex(void);
	~xregex(void);

public:
	static bool is_interge(const tstring& str);
	static bool is_id(const tstring& str);

public:
	static bool is_match(const tstring& regex_str, const tstring& content);
	static tstring replace_all(const tstring& regex_str, 
		const tstring& replace_str, 
		const tstring& org_str);
	//static bool is_match(const boost::regex& regex, const tstring& content);
};

} // end of namespace stringX

#endif
