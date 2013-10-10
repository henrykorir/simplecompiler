// folder.h : 
// 

#ifndef _FOLDER_H_KOG_
#define _FOLDER_H_KOG_

#include <string>
#include <macros.h>

#include "ioerror.h"

NAMESPACE_BEGIN(kog);

class folder
{
public:
	typedef std::string tstring;
public:
	folder(const tstring& folder_name);
	virtual ~folder();
public:
	bool isexist() const;
	void mkdir();
private:
	tstring path_;
};

struct filesystem
{
	typedef std::string tstring;

	static bool split_path(const tstring& input_path, tstring& folder_path, tstring& file_name)
	{
		if (input_path.empty()) return false;
#if (defined VISUAL_STDIO || defined _MSC_VER)
		size_t pos = input_path.find_last_of("/\\");
#else
		size_t pos = input_path.find_last_of("/");
#endif
		if (pos != tstring::npos)
		{ // eg: E:\abc\abc.txt
			folder_path = input_path.substr(0, pos);
			file_name = input_path.substr(pos + 1);
		}
		else if (input_path.find(':') != tstring::npos)
		{ // eg: E:
			folder_path = input_path;
			file_name = "";
		}
		else 
		{ // eg: abc.txt
			folder_path = "";
			file_name = input_path;
		}
		return true;
	}

	static bool split_title(const tstring& file_name, tstring& title, tstring& ext)
	{
		if (file_name.empty()) return false;
#if (defined VISUAL_STDIO || defined _MSC_VER)
		size_t pos = file_name.find_last_of("/\\.");
#else
		size_t pos = file_name.find_last_of("/.");
#endif
		if (pos != tstring::npos)
		{
			if (file_name[pos] == '.')
			{ // eg: E:\abc.txt
				ext = file_name.substr(pos);
#if (defined VISUAL_STDIO || defined _MSC_VER)
				size_t pos2 = file_name.find_last_of("/\\");
#else
				size_t pos2 = file_name.find_last_of("/");
#endif
				if (pos2 != tstring::npos)
				{ // eg: E:\abc.txt
					title = file_name.substr(pos2 + 1, pos - pos2 - 1);
				}
				else 
				{ // eg: abc.txt
					title = file_name.substr(0, pos);
				}
			}
			else if (file_name.size() == pos)
			{ // eg: E:\abc\def/
				return false;
			}
			else
			{
				title = file_name.substr(pos + 1);
				ext = "";
			}
		}
		else
		{
			title = file_name;
			ext = "";
		}
		return true;
	}

	static tstring path_cat(const tstring& folder_path, const tstring& file_name)
	{
		if (folder_path.empty()) return file_name;
		else if (file_name.empty()) return folder_path;

		char ch_end = folder_path[folder_path.size() - 1];
		char ch_beg = file_name[0];
#if (defined VISUAL_STDIO || defined _MSC_VER)
		bool is_end = ch_end == '\\' || ch_end == '/';
		bool is_beg = ch_beg == '\\' || ch_beg == '/';
#else
		bool is_end = ch_end == '/';
		bool is_beg = ch_beg == '/';
#endif
		if (is_beg && is_end) return folder_path + file_name.substr(1);
		else if (is_beg || is_end) return folder_path + file_name;
#if (defined VISUAL_STDIO || defined _MSC_VER)
		else return folder_path + "\\" + file_name;
#else
		else return folder_path + "/" + file_name;
#endif
	}
};

NAMESPACE_END(kog);

#endif
