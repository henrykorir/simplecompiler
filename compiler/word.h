// word.h : define word
//

#ifndef _WORD_H_SC_
#define _WORD_H_SC_

#include <vector>

#include <basic_types.h>
#include "object.h"

NAMESPACE_BEGIN(compile)

NAMESPACE_BEGIN(doc)

struct word : public compile::object
{
public:
	typedef sc::tstring _Str;
public:
	word(void) {}
	word(const _Str& str, short state = Unknown)
		: txt(str), wordstate(state)
	{}
public:
	enum{Known = 1, Unknown = 0, Error = -1};

public:
	void swap(word& aword)
	{
		txt.swap(aword.txt);
		std::swap(wordstate, aword.wordstate);
		std::swap(wordClass, aword.wordClass);
	}
public:
    /* overwrite */ virtual _Str to_string() const
    {
        return txt;
    }
public:
	_Str txt;
	short wordstate;
	short wordClass;
};

NAMESPACE_END(doc)

NAMESPACE_END(compile)

#endif
