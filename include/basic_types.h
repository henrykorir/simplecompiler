// basic_types.h : define basic types
//

#ifndef _BASIC_TYPES_H_SC_
#define _BASIC_TYPES_H_SC_

#include <macros.h>
#include <string>
#include <iostream>

#if (defined _WIN32 || defined WIN32)
#ifndef VISUAL_STDIO
#define VISUAL_STDIO
#endif
#endif

#ifndef _t
// if using unicode define _t(x) Lx
#define _t(x) x
#endif

NAMESPACE_BEGIN(sc)

typedef long int int64;
typedef int int32;
typedef short int16;
typedef char int8;
typedef unsigned long int uint64;
typedef unsigned int uint32;
typedef unsigned short uint16;
typedef unsigned char uint8;
typedef char tchar;
typedef std::string tstring;
typedef tstring _Str;

typedef unsigned char uchar;
typedef uchar byte;

typedef std::basic_istream<tchar> tistream;
typedef std::basic_ostream<tchar> tostream;

NAMESPACE_END(sc)

NAMESPACE_BEGIN(compile)
using namespace sc;
NAMESPACE_END(compile)

#include <scerror.h>
#include <logger.h>

#endif

