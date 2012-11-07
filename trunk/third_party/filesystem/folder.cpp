#include "folder.h"

//#include <cstdlib>

#if (defined _WIN32 || defined WIN32)
#include <io.h>
#include <Windows.h>
#ifndef WINDOWS
#define WINDOWS
#endif
#else
#include<unistd.h>
#include <sys/stat.h>
#include <errno.h>
#ifndef LINUX
#define LINUX
#endif
#endif

using namespace kog;

folder::folder(const tstring& folder_name)
: path_(folder_name)
{
}

folder::~folder()
{
}

bool folder::isexist() const
{
#ifdef WINDOWS
	return _access(path_.c_str(), 0) != -1;
#else
	return access(path_.c_str(), 0) == 0;
#endif
}

void folder::mkdir()
{
#ifdef WINDOWS
	if(CreateDirectoryA(path_.c_str(), NULL) == 0)
	{
		tstring reason;
		switch (GetLastError())
		{
		case ERROR_ALREADY_EXISTS:
			reason = "The specified directory already exists.";
			break;
		case ERROR_PATH_NOT_FOUND:
			reason = "One or more intermediate directories do not exist; this function will only create the final directory in the path.";
			break;
		default:
			reason = "unknown reason.";
		}
		throw ioerror("mkdir(" + path_ + ") failed! " + reason);
	}
#elif defined LINUX
	if(::mkdir(path_.c_str(), 0) == -1)
	{
		tstring reason;
		switch (errno)
		{
		case EACCES: 
			reason = "Write permission is denied for the parent directory in which the new directory is to be added."; 
			break;
		case EEXIST:
			reason = "A file named filename already exists.";
			break;
		case EMLINK:
			reason = "The parent directory has too many links (entries).";
			break;
		case ENOSPC:
			reason = "The file system doesn't have enough room to create the new directory.";
			break;
		case EROFS:
			reason = "The parent directory of the directory being created is on a read-only file system and cannot be modified.";
			break;
		default:
			reason = "unknown reason.";
		}
		throw ioerror("mkdir(" + path_ + ") failed! " + reason);
	}
#endif
}
