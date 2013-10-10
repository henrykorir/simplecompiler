// arraylist.h: array
// 

#ifndef _ARRAYLIST_H_SC_
#define _ARRAYLIST_H_SC_

#include <macros.h>
#include "type.h"
#include "variable.h"

#include <vector>

NAMESPACE_BEGIN(compile)
NAMESPACE_BEGIN(runtime)

class arraylist : public variable
{
public:
	typedef compile::object* object_ptr;
	typedef std::vector<object_ptr>::iterator iterator;
//public:
//	arraylist();
//	arraylist(object_ptr p, ...);
public:
	size_t size() const;

	iterator begin() { return objs_.begin(); }
	iterator end() { return objs_.end(); }
public:
	arraylist* append(const arraylist* other);
	arraylist* insert(const arraylist* other);
public:
	object_ptr& operator[](size_t idx);

	arraylist operator +(const arraylist& other) const;
	arraylist operator +(object_ptr other) const;

	arraylist& operator+=(const arraylist& other);
	arraylist& operator+=(object_ptr other);
public:
	/* overwrite */ virtual _Str to_string() const;
private:
	std::vector<object_ptr> objs_;
};

//extern arraylist* operator+ (const arraylist* v1, const arraylist* v2);
NAMESPACE_END(runtime)
NAMESPACE_END(compile)

#endif
