// stringX.h :
//

#ifndef _STRINGXF_H_HUXL_
#define _STRINGXF_H_HUXL_

#include <string>
#include <cstdarg>
#include <locale>
//#include <memory>
#include <sstream>
#include <functional>
#include <stdexcept>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <vector>
//#include <wtypes.h>

#include <macros.h>

#if (defined _MSC_VER) || defined(MSC_VER)
#ifndef MICROSIFT_VISUAL_STUDIO
#define MICROSIFT_VISUAL_STUDIO
#endif
#endif

NAMESPACE_BEGIN(stringX)

class strformat
{
public:
	static std::string format(const char* fmt, ...)
	{
		va_list argptr;
		va_start(argptr, fmt);
		std::string tmp;
		format(fmt, argptr, tmp);
		va_end(argptr);
		return tmp;
	}

	static std::wstring format(const wchar_t* fmt, ...)
	{
		va_list argptr;
		va_start(argptr, fmt);
		std::wstring str;
		format(fmt, argptr, str);
		va_end(argptr);
		return str;
	}

	static std::string& format(const char* fmt, va_list argptr, std::string& out_buf)
	{
#ifdef MICROSIFT_VISUAL_STUDIO
#pragma warning(push)
#pragma warning(disable : 4996)
#endif
		int bufsize = vsnprintf(NULL, 0, fmt, argptr) + 2;
		out_buf.clear();
		char* buf = new char[bufsize];
		try
		{
			bufsize = vsnprintf(buf, bufsize, fmt, argptr);
			out_buf.insert(out_buf.end(), buf, buf + bufsize);
		}catch(...){
		}
		delete []buf;
#ifdef MICROSIFT_VISUAL_STUDIO
#pragma warning(pop)
#endif
		return out_buf;
	}

	static std::wstring& format(const wchar_t* fmt, va_list argptr, std::wstring& out_buf)
	{
#ifdef MICROSIFT_VISUAL_STUDIO
#pragma warning(push)
#pragma warning(disable : 4996)
#endif
		int bufsize = vswprintf(NULL, 0, fmt, argptr) + 2;
		out_buf.clear();
		wchar_t* buf = new wchar_t[bufsize];
		try
		{
			bufsize = vswprintf(buf, bufsize, fmt, argptr);
			out_buf.insert(out_buf.end(), buf, buf + bufsize);
		}catch(...){
		}
		delete []buf;
#ifdef MICROSIFT_VISUAL_STUDIO
#pragma warning(pop)
#endif
		return out_buf;
	}
	//static int scanf(const char* source, const char* fmt, ...)
	//{
	//	va_list argptr;
	//	va_start(argptr, fmt);

	//	#pragma warning(push)
	//	#pragma warning(disable : 4996)
	//
	//	//sscanf(source, fmt
	//	#pragma warning(pop)
	//	va_end(argptr);
	//}
};

template<typename _Elem>
std::basic_string<_Elem> format(const _Elem* _fmt, ...)
{
	va_list argptr;
	va_start(argptr, _fmt);
	std::basic_string<_Elem> buf;
	strformat::format(_fmt, argptr, buf);
	va_end(argptr);
	return buf;
}

template<typename _Elem>
std::basic_string<_Elem> format(const std::basic_string<_Elem>& _fmt, ...)
{
	va_list argptr;
	va_start(argptr, _fmt);
	std::basic_string<_Elem> buf;
	strformat::format(_fmt.c_str(), argptr, buf);
	va_end(argptr);
	return buf;
}

template<typename _Elem>
std::basic_string<_Elem> replacefirst(const std::basic_string<_Elem>& str, const std::basic_string<_Elem>& oldString, const std::basic_string<_Elem>& newString, size_t pos = 0)
{
	std::string temp = str;
	pos = temp.find(oldString, pos);
	if(pos != std::string::npos)
		temp.replace(pos, oldString.size(), newString);
	return temp;
}

template<typename _Elem>
std::basic_string<_Elem> replaceall(const std::basic_string<_Elem>& str, 
									const std::basic_string<_Elem>& oldString, 
									const std::basic_string<_Elem>& newString)
{
	size_t pos = 0;
	std::string temp(str);
	pos = temp.find(oldString, pos);
	while(pos != std::string::npos)
	{
		temp.replace(pos, oldString.size(), newString);
		pos = temp.find(oldString, pos + newString.size());
	}
	return temp;
}

template<class _Elem>
struct string_split_t
{
    string_split_t(const std::basic_string<_Elem>& str, const std::basic_string<_Elem>& space, bool is_remove_empty = true)
    : main_(str)
    , space_(space)
    , isre_(is_remove_empty)
    {
        if (space_.empty()) throw std::runtime_error("empty space!");
    }

    struct _Const_iterator
    {
    public:
        _Const_iterator(string_split_t<_Elem>* ref, size_t posB)
            : ref_(ref)
            , posB_(posB)
        {
            posE_ = _next(posB_);
            while (ref_->isre_ && posB_ == posE_ && posE_ != ref_->main_.length())
            {
                posB_ += ref_->space_.length();
                posE_ = _next(posB_);
            }
        }

        _Const_iterator(string_split_t<_Elem>* ref, size_t posB, size_t posE)
            : ref_(ref)
            , posB_(posB)
            , posE_(posE)
        {
        }

        _Const_iterator(const _Const_iterator& other)
            : ref_(other.ref_)
            , posB_(other.posB_)
            , posE_(other.posE_)
        {
        }
    public:
        friend bool operator== (const _Const_iterator& iter1, const _Const_iterator& iter2)
        {
            if (iter1.ref_ != iter2.ref_)
                throw std::runtime_error("string_split::const_iterator, ref not equal!");
            return iter1.posB_ == iter2.posB_;
        }
        
        friend bool operator!= (const _Const_iterator& iter1, const _Const_iterator& iter2)
        {
            return !(iter1 == iter2);
        }

    public:
        std::basic_string<_Elem> operator*()
        {
            return ref_->main_.substr(posB_, posE_ - posB_);
        }
    public:
        _Const_iterator& operator++()
        {
            if (posB_ >= ref_->main_.length())
                throw std::out_of_range("string_split::const_iteator::operator++");
            else if(posE_ == ref_->main_.length())
                posB_ = posE_;
            else
            {
                do {
                    size_t pos = posE_ + ref_->space_.length();
                    posB_ = pos;
                    posE_ = _next(posB_);
                }while(ref_->isre_ && posE_ == posB_ && posE_ != ref_->main_.length());
            }
			return *this;
        }

        _Const_iterator operator++(int)
        {
            _Const_iterator tmp(*this);
            ++ (*this);
            return tmp;
        }
    private:
        size_t _next(size_t posb)
        {
	        size_t pos = ref_->main_.find(ref_->space_, posb);
            if (pos == std::basic_string<_Elem>::npos) pos = ref_->main_.length();
            return pos;
        }
    private:
        string_split_t<_Elem>* ref_;
        size_t posB_;
        size_t posE_;
    };
public:
    typedef _Const_iterator const_iterator;
    friend struct _Const_iterator;
public:
    const_iterator begin()
    {
        _Const_iterator iter(this, 0);
        return iter;
    }

    const_iterator end()
    {
        _Const_iterator iter(this, main_.length(), main_.length());
        return iter;
    }
private:
    const std::basic_string<_Elem> main_;
    const std::basic_string<_Elem> space_;
    bool isre_;
};

template<typename _Elem>
std::vector<std::basic_string<_Elem> > split(const std::basic_string<_Elem>& str, const std::basic_string<_Elem>& space, bool is_remove_empty = false)
{
	std::vector<std::basic_string<_Elem> > vtmp;
	if(space.empty())
	{
		for(size_t i = 0; i < str.size(); ++ i) vtmp.push_back(std::basic_string<_Elem>(1, str[i]));
		return vtmp;
	}
	size_t pos_p = 0;
	size_t pos = str.find(space, pos_p);
	while(pos != std::string::npos)
	{
		if(!(pos == pos_p && is_remove_empty))
			vtmp.push_back(str.substr(pos_p, pos - pos_p));
		pos_p = pos + space.size();
		pos = str.find(space, pos_p);
	}
	if(pos_p == str.size() && is_remove_empty) return vtmp;
	vtmp.push_back(str.substr(pos_p));
	return vtmp;
}

template<typename _Ty, typename _Elem> _Ty tovalue(const _Elem* ptr)
{
	if(NULL == ptr) throw std::runtime_error("empty string!");
	return tovalue(std::basic_string<_Elem>(ptr));
}

template<typename _Ty, typename _Elem> _Ty tovalue(const std::basic_string<_Elem>& s)
{
	std::basic_stringstream<_Elem> ss(s);
	_Ty temp;
	ss>>temp;
	if(ss.fail())
	{
		std::string buf("can't convert string to value!");
		throw std::runtime_error(buf);
	}
	return temp;
}

/*
template<typename _Ty1, typename _Ty2, typename _Elem>
_Ty2 Convert(const _Ty1& _arg)
{
	std::basic_stringstream<_Elem> ss;
	ss<<_arg;
	_Ty2 temp;
	ss>>temp;
	return temp;
}*/

template<typename _Elem>
_Elem tolower(_Elem _ch)
{
	int temp = _ch - 'a';
	return (_ch - 'a') >=0 ? _ch : (_ch - 'A' + 'a');
}

template<typename _Elem>
_Elem toupper(_Elem _ch)
{
	int temp = _ch - 'a';
	return (_ch - 'a') >=0 ? (_ch - 'a' + 'A') : _ch;
}

template<typename _Elem>
std::basic_string<_Elem> tolower(const std::basic_string<_Elem>& _Str)
{
	std::basic_string<_Elem> tmp(_Str.size(), (_Elem)'\0');
	for(size_t i = 0; i < tmp.size(); ++ i)
		if(/*std*/::isupper(_Str[i])) tmp[i] = (_Str[i] - 'A') + 'a';
		else tmp[i] = _Str[i];
	return tmp;
}

template<typename _Elem>
std::basic_string<_Elem> toupper(const std::basic_string<_Elem>& _Str)
{
	std::basic_string<_Elem> tmp(_Str.size(), (_Elem)'\0');
	for(size_t i = 0; i < tmp.size(); ++ i)
		if(::islower(_Str[i])) tmp[i] = (_Str[i] - 'a') + 'A';
		else tmp[i] = _Str[i];
	return tmp;
}
/*inline bool AWEqual(const std::string& str1, const std::wstring& str2)
{
	std::string str = w2a(str2);
	return str == str1;
}*/

template<typename _CmpFunc, typename _Elem>
struct stringcmp : public std::binary_function<std::basic_string<_Elem>, std::basic_string<_Elem>, bool>
{
	bool operator()(const std::basic_string<_Elem>& str1, const std::basic_string<_Elem>& str2) const
	{
		return _fun(str1, str2);
	}

	_CmpFunc _fun;
};

namespace Format
{
	struct IsDigit
	{
		bool operator()(char _Ch) const
		{
			return _Ch >= '0' && _Ch <= '9';
		}

		bool operator()(wchar_t _Ch) const
		{
			return _Ch >= L'0' && _Ch <= L'9';
		}
	};

	struct IsSpace
	{
		bool operator()(char _Ch) const
		{
			unsigned short i = (unsigned short)_Ch;
			return i == ' ' || i == '\t' || 
				i == '\n' || i == '\v' || i == '\f' || i == '\r';
		}

		bool operator()(wchar_t _Ch) const
		{
			unsigned short i = (unsigned short)_Ch;
			return i == L' ' || i == L'\t' || 
				i == L'\n' || i == L'\v' || i == L'\f' || i == L'\r';
		}
	};

	template<typename _Elem>
	_Elem meta(unsigned short _ch)
	{
		return (_Elem)_ch;
	}

	struct swcvt
	{
		static bool convert(const std::string& _Src, std::wstring& _Dst)
		{
			std::exception ex;
			return tryconvert(_Src, _Dst, ex);
		}

		static bool convert(const std::string& _Src, std::string& _Dst)
		{
			_Dst = _Src;
			return true;
		}

		static std::wstring convert(const std::string& _Src)
		{
			std::exception ex;
			std::wstring buf;
			if(!tryconvert(_Src, buf, ex))
				throw ex;
			return buf;
		}

		static bool convert(const std::wstring& _Src, std::string& _Dst)
		{
			std::exception ex;
			return tryconvert(_Src, _Dst, ex);
		}

		static bool convert(const std::wstring& _Src, std::wstring& _Dst)
		{
			_Dst = _Src;
			return true;
		}

		static std::string convert(const std::wstring& _Src)
		{
			std::exception ex;
			std::string buf;
			if(!tryconvert(_Src, buf, ex))
				throw ex;
			return buf;
		}

		template<typename _Dst_Elem, typename _Src_Elem>
		static std::basic_string<_Dst_Elem> convert(const std::basic_string<_Src_Elem>& _Src)
		{
			std::basic_string<_Dst_Elem> tmp;
			convert(_Src, tmp);
			return tmp;
		}

	private:
		static bool tryconvert(const std::wstring& _Src, std::string& _Dst, std::exception& ex)
		{
			if(_Src.empty())
			{
				_Dst = std::string();
				return true;
			}
			else if(_Src == L"")
			{
				_Dst = "";
				return true;
			}

			std::locale sys_loc("");

			const wchar_t* src_wstr = _Src.c_str();
			const size_t MAX_UNICODE_BYTES = 4;//_Src.size() / _Src.length();
			const size_t BUFFER_SIZE =
				_Src.length() * MAX_UNICODE_BYTES + 1;

			//std::auto_ptr<char> buf(new char[BUFFER_SIZE]);
			std::string buf(BUFFER_SIZE, '\0');
			char* extern_buffer = (char*)buf.c_str();
			memset(extern_buffer, 0, BUFFER_SIZE);

			const wchar_t* intern_from = src_wstr;
			const wchar_t* intern_from_end = intern_from + _Src.size();
			const wchar_t* intern_from_next = 0;
			char* extern_to = extern_buffer;
			char* extern_to_end = extern_to + BUFFER_SIZE;
			char* extern_to_next = 0;

			typedef std::codecvt<wchar_t, char, mbstate_t> CodecvtFacet;

			mbstate_t out_cvt_state;
			CodecvtFacet::result cvt_rst =
				std::use_facet<CodecvtFacet>(sys_loc).out(
					out_cvt_state,
					intern_from, intern_from_end, intern_from_next,
					extern_to, extern_to_end, extern_to_next);
			if (cvt_rst != CodecvtFacet::ok) {
				std::string exceptionMsg =
					"an exception occur when convert wstring to string: the exception is ";

				switch(cvt_rst) {
					case CodecvtFacet::partial:
						exceptionMsg += "convert partial";
						break;
					case CodecvtFacet::error:
						exceptionMsg += "error";
						break;
					case CodecvtFacet::noconv:
						exceptionMsg += "noconv";
						break;
					default:
						exceptionMsg += "unknown";
				}
				ex = std::runtime_error(exceptionMsg.c_str());
				return false;
			}

			_Dst = buf.c_str();
			return true;
		}

		static bool tryconvert(const std::string& _Src, std::wstring& _Dst, std::exception& ex)
		{
			if(_Src.empty())
			{
				_Dst = std::wstring();
				return true;
			}
			else if(_Src == "")
			{
				_Dst = L"";
				return true;
			}

			std::locale sys_loc("");

			const char* src_str = _Src.c_str();
			const size_t BUFFER_SIZE = _Src.size() + 1;

			//std::auto_ptr<wchar_t> buf(new wchar_t[BUFFER_SIZE]);
			std::wstring buf(BUFFER_SIZE, L'\0');
			wchar_t* intern_buffer = (wchar_t*)buf.c_str();
			wmemset(intern_buffer, 0, BUFFER_SIZE);

			const char* extern_from = src_str;
			const char* extern_from_end = extern_from + _Src.size();
			const char* extern_from_next = 0;
			wchar_t* intern_to = intern_buffer;
			wchar_t* intern_to_end = intern_to + BUFFER_SIZE;
			wchar_t* intern_to_next = 0;

			typedef std::codecvt<wchar_t, char, mbstate_t> CodecvtFacet;

			mbstate_t in_cvt_state;
			CodecvtFacet::result cvt_rst =
				std::use_facet<CodecvtFacet>(sys_loc).in(
					in_cvt_state,
					extern_from, extern_from_end, extern_from_next,
					intern_to, intern_to_end, intern_to_next);

			if (cvt_rst != CodecvtFacet::ok) {
				std::string exceptionMsg =
					"an exception occur when convert string \"" + _Src + "\" to wstring: the exception is ";
				switch(cvt_rst) {
					case CodecvtFacet::partial:
						exceptionMsg += "convert partial";
						break;
					case CodecvtFacet::error:
						exceptionMsg += "error";
						break;
					case CodecvtFacet::noconv:
						exceptionMsg += "noconv";
						break;
					default:
						exceptionMsg += "unknown";
				}
				ex = std::runtime_error(exceptionMsg.c_str());
				return false;
			}

			_Dst = buf.c_str();
			return true;
		}
	};
}

// erase the pad spaces of the string
// param@ path, the original string
// param@ trimType
//		trimType == 1 : erase the ending spaces
//		trimType == 2 : erase the leading spaces
//		trimType == 3 : erase the leading and ending spaces
inline std::string trim(const std::string& path, int trimType = 3)
{
	size_t pos = 0;
	size_t len = path.size();

	if(trimType & 0x01)
	{
		while(len && Format::IsSpace()(path[len-1])) -- len;
	}
	if(trimType & 0x02)
	{
		while(len && Format::IsSpace()(path[pos])) ++ pos, -- len;
	}
	return path.substr(pos, len);
}


// check string(main) is prefix with string(pattern)
template<typename _Char>
bool is_prefix_with(const std::basic_string<_Char>& main, const std::basic_string<_Char>& pattern)
{
	if (main.size() < pattern.size()) return false;
	size_t n = pattern.size();
	const _Char* pm = main.c_str();
	const _Char* pp = pattern.c_str();
	const int* pim = (const int*)pm;
	const int* pip = (const int*)pp;
	size_t m = n / sizeof(int);
	while (m && *(pim ++) == *(pip ++)) -- m;
	if (0 == m)
	{
		n %= sizeof(int);
		pm = (const _Char*)pim;
		pp = (const _Char*)pip;
		while(n && *(pm ++) == *(pp ++)) --n;
	}
	return m == 0 && n == 0;

}

//template<typename _Char>
//bool prefix_with(const _Char* main, const _Char* pattern)
//{
//	if (main == NULL || pattern == NULL) throw std::runtime_error("invalidate param");
//	while ()
//}

typedef stringcmp<std::less<std::string>, char> stringless; 
NAMESPACE_END(stringX)

#endif
