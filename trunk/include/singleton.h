// singleton.h: singleton
//

#ifndef _SINGLETON_H_HUXL_
#define _SINGLETON_H_HUXL_

#include <macros.h>

NAMESPACE_BEGIN(kog)

#ifndef NON_COPYABLE_OBJECT
#define NON_COPYABLE_OBJECT(classname) \
		private: \
			classname& operator=(const classname&); \
			classname(const classname&);
#endif

template<class _T>
class singleton
{
public:
	typedef singleton<_T> singleton_type;
	typedef _T value_type;
protected:
	singleton(){}
	~singleton(){}

public:
	static value_type& instance()
	{
		static value_type anInst;
		return anInst;
	}

private:
	singleton_type& operator=(const singleton_type& other);
	singleton(const singleton_type& other);
};

NAMESPACE_END(kog)
#endif
