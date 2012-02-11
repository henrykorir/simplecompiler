// extract.h: get words from input stream
//

#ifndef _EXTRACT_H_SC_
#define _EXTRACT_H_SC_

#include <deque>

#include <macros.h>
#include <basic_types.h>

#include "word.h"

NAMESPACE_BEGIN(compile)

NAMESPACE_BEGIN(doc)

class streamsplit
{
public:
	typedef std::deque<word> deqwords;
public:
	streamsplit();
	virtual ~streamsplit();
public:
	MEMBER_VARIABLE_GET(const deqwords&, words, words_);
public:
	deqwords& operator()(std::istream& is);
private:
	deqwords words_;
};

NAMESPACE_END(doc)
NAMESPACE_END(compile)
#endif

