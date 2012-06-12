// buckethash.h: 
//

#ifndef _BUCKETHASH_H_HUXL_
#define _BUCKETHASH_H_HUXL_

#include <macros.h>
#include <arrayX.h>
#include <functionalX.h>
#include <list>

NAMESPACE_BEGIN(kog)

template<typename _T>
struct bucket : public std::list<_T>
{
};

class hash_error : public std::runtime_error
{
public:
	hash_error()
		: std::runtime_error("")
	{
	}

	hash_error(const std::string& err)
		: std::runtime_error(err)
	{
	}
};

template<typename _K, typename _V, typename _K2I>
class buckethash
{
public:
	typedef  _K key;
	typedef  _V value;
	typedef  value& value_reference;
private:
	typedef std::pair<key, value> keyvalue;
	typedef typename bucket<keyvalue>::iterator bucket_iterator;
	typedef typename bucket<keyvalue>::const_iterator bucket_const_iterator;

	friend struct _Const_iterator;
	friend struct _Iterator;
public:
	buckethash(std::size_t _MaxLength)
	: buckets_(_MaxLength)
	{}
public:
	struct _Const_iterator
	{
	private:
		_Const_iterator(const buckethash<_K, _V, _K2I>* p)
		: ref(p)
		, kid(-1)
		{}
	public:
		_Const_iterator()
		: ref(NULL), kid(-1)
		{}

//		operator ++(int)
//		{
//		}
	private:
		size_t kid;
		bucket_const_iterator iter;
		const buckethash<_K, _V, _K2I>* ref;
	};
public:
	value& operator[](const key& k)
	{
		size_t kid = _bucket_index(k);
		bucket_iterator iter_find = _bucket_Locate(kid, k);
		if (iter_find == buckets_[kid].end()) 
			throw hash_error("not found key in bucket!");
		
		return iter_find->second;
	}

	const value& operator[](const key& k) const
	{
		size_t kid = _bucket_index(k);
		bucket_const_iterator iter_find = _bucket_Locate(kid, k);
		if (iter_find == buckets_[kid].end()) 
			throw hash_error("not found key in bucket!");
		
		return iter_find->second;
	}

public:
	void insert(const key& k, const value& v)
	{
		size_t kid = _bucket_index(k);
		bucket_iterator iter_find = _bucket_Locate(kid, k);
		if (iter_find != buckets_[kid].end()) 
			throw hash_error("found exisited key in bucket!");
		buckets_[kid].push_back(keyvalue(k, v));
	}

	void insert(const keyvalue& kv)
	{
		insert(kv.first, kv.second);
	}

	void remove(const key& k)
	{
		size_t kid = _bucket_index(k);
		bucket_const_iterator iter_find = _bucket_Locate(kid, k);
		if (iter_find == buckets_[kid].end()) 
			throw hash_error("not found key in bucket!");
		buckets_[kid].erase(iter_find);
	}
public:
	value* find(const key& k)
	{
		try{
			value& v = (*this)[k];
			return &v;
		}catch(...){
		}
		return NULL;
	}

	const value* find(const key& k) const
	{
		try{
			const value& v = (*this)[k];
			return &v;
		}catch(...){
		}
		return NULL;
	}

private:
	size_t _bucket_index(const key& k) const
	{
		_K2I key2index;
		std::size_t idx = key2index(k);
		if(idx >= buckets_.size()) 
			throw hash_error("invalidate index!");
		return idx;
	}

	bucket_const_iterator _bucket_Locate(size_t kid, const key& k) const
	{
		return std::find_if(buckets_[kid].begin(), buckets_[kid].end(),
			kog::composite_function(kog::mem_value(&keyvalue::first), 
				std::bind2nd(std::equal_to<key>(), k)));
		
	}

	bucket_iterator _bucket_Locate(size_t kid, const key& k)
	{
		return std::find_if(buckets_[kid].begin(), buckets_[kid].end(),
			kog::composite_function(kog::mem_value(&keyvalue::first), 
				std::bind2nd(std::equal_to<key>(), k)));
	}
private:
	kog::smart_vector<bucket<keyvalue> > buckets_;
};

NAMESPACE_END(kog)

#endif
