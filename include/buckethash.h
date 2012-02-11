// buckethash.h: 
//

#ifndef _BUCKETHASH_H_HUXL_
#define _BUCKETHASH_H_HUXL_

#include <macros.h>
#include <arrayX.h>
#include <list>

NAMESPACE_BEGIN(kog)

template<typename _T>
struct bucket : public std::list<_T>
{
};

template<typename _K, typename _V, typename _K2I>
class buckethash
{
public:
	typedef typename _K key;
	typedef typename _V value;
	typedef typename value& value_reference;
public:
	buckethash(std::size_t _MaxLength)
	: buckets_(_MaxLength)
	{}
public:
	value& operator[](const key& k)
	{
		_K2I key2index;
		std::size_t idx = key2index(k);
		if(idx >= buckets_.size()) throw std::runtime_error("invalidate index!");
		return buckets_[idx].front();
	}
private:
	kog::smart_vector<bucket<std::pair<key, value> > > buckets_;
};

NAMESPACE_END(kog)
#endif
