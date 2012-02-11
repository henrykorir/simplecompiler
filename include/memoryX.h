// memoryX.h: define memory structs and functions
//

#ifndef _MEMORYX_H_HUXL_
#define _MEMORYX_H_HUXL_

#include <macros.h>
#include <iteratorX.h>

NAMESPACE_BEGIN(kog)

template<typename _Tx>
struct memblock
{
public:
	typedef _Tx value_type;
	typedef value_type* pointer;
	typedef const value_type* const_pointer;
	typedef value_type& reference;
	typedef const value_type& const_reference;

	typedef std::size_t size_type;

	//typedef stdext::checked_array_iterator<array_iterator<value_type>> iterator;
	//typedef stdext::checked_array_iterator<array_iterator<const value_type>> const_iterator;
	typedef array_iterator<value_type> iterator;
	typedef array_iterator<const value_type> const_iterator;

	typedef std::reverse_iterator<iterator> reverse_iterator;
	typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
private:
	_Tx* v;
};

NAMESPACE_END(kog)

#endif
