#include "arraylist.h"

#include <cstdarg>
#include <cstdlib>
#include <sstream>
#include <functionalX.h>

using namespace compile;
using namespace compile::runtime;


//arraylist::arraylist()
//{
//}
//
//arraylist::arraylist(object_ptr p, ...)
//{
//	va_list argptr;
//	va_start(argptr, p);
//	
//	while (p != NULL)
//	{
//		objs_.push_back(p);
//		++ p;
//	}
//	
//	va_end(argptr);
//}

size_t arraylist::size() const
{
	return objs_.size();
}

arraylist::object_ptr& arraylist::operator[](size_t idx)
{
	return objs_.at(idx);
}

arraylist arraylist::operator +(const arraylist& other) const
{
	arraylist tmp(*this);
	tmp.objs_.insert(tmp.objs_.end(), other.objs_.begin(), other.objs_.end());
	return tmp;
}

arraylist arraylist::operator +(object_ptr other) const
{
	arraylist tmp(*this);
	tmp.objs_.push_back(other);
	return tmp;
}

arraylist& arraylist::operator+=(const arraylist& other)
{
	objs_.insert(objs_.end(), other.objs_.begin(), other.objs_.end());
	return *this;
}


arraylist& arraylist::operator+=(object_ptr other)
{
	objs_.push_back(other);
	return *this;
}
	
arraylist* arraylist::append(const arraylist* other)
{
	if (other == NULL) return this;
	*this += *other;
	return this;
}

arraylist* arraylist::insert(const arraylist* other)
{
	if (other == NULL) return this;
	objs_.insert(objs_.begin(), other->objs_.begin(), other->objs_.end());
	return this;
}

_Str arraylist::to_string() const
{
	const tchar* sep = "";
	std::ostringstream oss;
	oss<<"{";
	foreach (object_ptr p, objs_.begin(), objs_.end())
	{
		oss<<sep<<(p == NULL ? "(null)" : p->to_string());
		sep = ", ";
	}
	oss<<"}";
	return oss.str();
}


//
//arraylist* operator+ (const arraylist* v1, const arraylist* v2)
//{
//	arraylist* tmp = new arraylist(*v1);
//	(*tmp) += *v2;
//	return tmp;
//}
