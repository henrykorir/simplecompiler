// basic_types.h : define basic types
//

#ifndef _BASIC_TYPES_H_SC_
#define _BASIC_TYPES_H_SC_

#include <macros.h>
#include <string>

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

NAMESPACE_END(sc)

NAMESPACE_BEGIN(compile)
using namespace sc;
NAMESPACE_END(compile)

#endif

