// buckethash.h: 
//

#ifndef _BUCKETHASH_H_HUXL_
#define _BUCKETHASH_H_HUXL_

#include <macros.h>
#include <arrayX.h>
#include <functionalX.h>
#include <iterator>
#include <cstring>

NAMESPACE_BEGIN(kog)

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

template<typename _K, typename _V, typename _K2I, typename _Key_equal = std::equal_to<_K> >
class buckethash
{
public:
	typedef  _K key_type;
	typedef  _V value_type;
	typedef  value_type& value_reference;
	typedef  const value_type& const_value_reference;
	typedef std::pair<key_type, value_type> keyvalue_pair;
private:
	// double link list
	struct bucket
	{
		struct bucket_node
		{
			keyvalue_pair data; // stored data
			bucket_node* next; // next key-value
			bucket_node* proi; // proi pair

			bucket_node(const keyvalue_pair& kv)
				: data(kv), next(NULL), proi(NULL)
			{}

			bucket_node(const key_type& k, const value_type& v)
				: data(k, v), next(NULL), proi(NULL)
			{}
		};

		size_t count; 
		bucket_node* first; // first key-value pair
	};
private:
	
	typedef typename bucket::bucket_node* bucket_data_pointer;
	typedef typename bucket::bucket_node const* bucket_data_const_pointer;
	typedef unsigned int bitmap_item;
private:
	friend struct _Const_iterator; // iterator can acess private data of this
	friend struct _Iterator;
public:
	buckethash(std::size_t _MaxLength)
	: buckets_(_MaxLength)
	, bitmap_((_MaxLength + sizeof(bitmap_item) - 1) / sizeof(bitmap_item) / 8, 0)
	, count_(0)
	{
		if (_MaxLength == 0) throw hash_error("count of bucket can't be zero");
		memset(buckets_.get(), 0, buckets_.size_in_bytes());
	}

	virtual ~buckethash()
	{
		clear();
	}

	buckethash(const buckethash& _other)
	{
		// copy
		this->copy_from(_other);
	}
public:
	// const iterator, support ++ and --
	struct _Const_iterator : public std::iterator<std::bidirectional_iterator_tag, keyvalue_pair>
	{
		friend class buckethash; // use its private constructor
	public:
		// pointer and reference typedef 
		typedef const keyvalue_pair* const_pointer;
		typedef const keyvalue_pair& const_reference;
		typedef keyvalue_pair* pointer;
		typedef keyvalue_pair& reference;
	protected:
		_Const_iterator(const buckethash* r, size_t key_id, const typename bucket::bucket_node* p)
			: ref(r)
			, kid(key_id)
			, iter(p)
		{}
	public:
		_Const_iterator()
			: ref(NULL), kid(-1), iter(NULL)
		{}

		_Const_iterator(const _Const_iterator& _other)
			: ref(_other.ref), kid(_other.kid), iter(_other.iter)
		{
		}
	public:
		_Const_iterator& operator++()
		{
			if (ref == NULL || kid >= ref->buckets_.size() || iter == NULL)
				throw hash_error("invalidate iterator, out of range?");
			if (ref->buckets_[kid].first == iter->next)
			{
				// rearch end of current bucket
				// wrap to next bucket
				_wrap_next();
			}
			else
			{
				iter = iter->next;
			}
			return *this;
		}

		_Const_iterator operator ++(int)
		{
			_Const_iterator _tmp(*this);
			++ (*this);
			return _tmp;
		}

		_Const_iterator& operator--()
		{
			if (ref == NULL || kid > ref->buckets_.size())
				throw hash_error("invalidate iterator, out of range?");
			// begin of the hash set. 
			//   if count of hashset's buckets is 0, iter==NULL must be true
			//   so, don't need to care ref->buckets_[0] fire an exception
			else if (kid == 0 && (iter == NULL || iter->proi == ref->buckets_[0].first))
				throw hash_error("current iterator is at the begin of the hash set");
			else if (kid == ref->buckets_.size() && iter != NULL)
				throw hash_error("invalidate iterator, invalidate status");
			// rearch the begin of current bucket
			if (kid == ref->buckets_.size() || ref->buckets_[kid].first == iter->proi)
			{
				// rearch front of current bucket, wrap to proi bucket
				// may be fire an exception here, if current is the first pair of the hashset
				_wrap_proi();
			}
			else
			{
				iter = iter->proi; // move front
			}
			return *this;
		}

		_Const_iterator operator --(int)
		{
			_Const_iterator _tmp(*this);
			-- (*this);
			return _tmp;
		}

	public:
		const_reference operator*() const
		{
			if (iter == NULL)
				throw hash_error("invalidate iterator!");
			return iter->data;
		}

		const_pointer operator->() const
		{
			if (iter == NULL)
				throw hash_error("invalidate iterator!");
			return &(iter->data);
		}
	private:
		void move_to_first()
		{
			if (ref == NULL) throw hash_error("invalidate iterator");
			kid = 0;
			iter = NULL;
			if (ref->buckets_.size() == 0) return;
			if (ref->buckets_[0].first != NULL)
			{
				iter = ref->buckets_[0].first;
			}
			else 
			{
				_wrap_next();
			}
		}
	public:
		// support equal and notequal compair
		friend bool operator ==(const _Const_iterator& i1, const _Const_iterator& i2)
		{
			return i1.ref == i2.ref && i1.iter == i2.iter && i1.kid == i2.kid;
		}

		friend bool operator !=(const _Const_iterator& i1, const _Const_iterator& i2)
		{
			return !(i1 == i2);
		}
	private:
		// find next bucket has key-value pair, and return the first pair of the bucket
		void _wrap_next()
		{
			if (kid == ref->buckets_.size() - 1) // to ref->end()
			{
				++ kid; iter = NULL;
			}
			else
			{
				kid = _next_bucket(kid + 1); // find next bucket has pair using bitmap
				iter  = (kid >= ref->buckets_.size()) ? NULL : (ref->buckets_[kid].first);
			}
		}

		void _wrap_proi()
		{
			// kid must not be 0, beacuse operator--() has check it
			kid = _proi_bucket(kid - 1); // may return -1, if not bucket found
			if (kid >= ref->buckets_.size()) // not bucket found
			{
				throw hash_error("current iterator is at the begin of the hash set");
			}
			iter = ref->buckets_[kid].first->proi;
		}

		size_t _proi_bucket(size_t kid) const
		{
			size_t i = kid / (sizeof(bitmap_item) * 8);
			size_t j = kid - i * sizeof(bitmap_item) * 8;
			
			bitmap_item mask = -1 << (sizeof(bitmap_item) * 8 - 1 - j);
			j = -1;
			if (ref->bitmap_[i] & mask) j = _proi_nonzero(ref->bitmap_[i] & mask);
			else
			{
				for (-- i; i != -1 && ref->bitmap_[i] == 0; --i);
				if (i != -1) j = _proi_nonzero(ref->bitmap_[i]);
			}
			// i, j
			if (j == -1) return -1;
			return i * sizeof(bitmap_item) * 8 + j;
		}

		size_t _next_bucket(size_t kid) const
		{
			bitmap_item x = 0;
			size_t i = kid / (sizeof(bitmap_item) * 8);
			size_t j = kid - i * sizeof(bitmap_item) * 8;
			bitmap_item mask = -1;
			mask &= ~(-1 << (sizeof(bitmap_item)*8 - j));
			size_t t = sizeof(bitmap_item) * 8 - 1;
			if (mask & ref->bitmap_[i]) t = _next_nonzero(mask & ref->bitmap_[i]);
			else 
			{
				for (++i; i < ref->bitmap_.size() && ref->bitmap_[i]; ++ i);
				if (i <  ref->bitmap_.size()) t = _next_nonzero(ref->bitmap_[i]);
			}
			if (t == sizeof(bitmap_item) * 8 - 1) return ref->buckets_.size();
			return i * sizeof(bitmap_item) * 8 + t;
		}

		size_t _next_nonzero(bitmap_item mask) const
		{
			int i = sizeof(bitmap_item) * 8;
			bitmap_item j = 0x01 << (sizeof(bitmap_item) * 8 - 1);
			while (i)
			{
				if (j & mask) return (sizeof(bitmap_item) * 8 - i);
				j >>= 1;
				-- i;
			}
			return sizeof(bitmap_item) * 8 - 1;
		}

		size_t _proi_nonzero(bitmap_item mask) const
		{
			int i = sizeof(bitmap_item) * 8;
			bitmap_item j = 0x01;
			while (i)
			{
				if (j & mask) return i - 1;
				j <<= 1;
				-- i;
			}
			return -1;
		}
	protected:
		size_t kid;
		const typename bucket::bucket_node* iter;
		const buckethash* ref; // ref to hashset
	};

	struct _Iterator : public _Const_iterator
	{
		friend class buckethash;//<_K, _V, _K2I, _Key_equal>;
	public:
		typedef keyvalue_pair& reference;
		typedef keyvalue_pair* pointer;
	protected:
		_Iterator(const buckethash* r, size_t key_id, const typename bucket::bucket_node* p)
			: _Const_iterator(r, key_id, p)
		{}
	public:
		_Iterator()
		{}

		_Iterator(const _Iterator& _other)
			: _Const_iterator(_other)
		{
		}
	public:
		reference operator*() const
		{
			if (this->iter == NULL)
				throw hash_error("invalidate iterator!");
			return ((typename bucket::bucket_node*)this->iter)->data;
		}

		pointer operator->() const
		{
			if (this->iter == NULL)
				throw hash_error("invalidate iterator!");
			return (pointer)&(this->iter->data);
		}
	public:
		_Iterator& operator++()
		{
			++(*((_Const_iterator*)this));
			return *this;
		}

		_Iterator operator++(int)
		{
			_Iterator _tmp(*this);
			++ (*this);
			return _tmp;
		}

		_Iterator& operator--()
		{
			--(*((_Const_iterator*)this));
			return *this;
		}

		_Iterator operator--(int)
		{
			_Iterator _tmp(*this);
			-- (*this);
			return _tmp;
		}
	};
public:
	typedef _Const_iterator const_iterator;
	typedef _Iterator iterator;
	// reverse iterator is not effictive
	//typedef std::reverse_bidirectional_iterator<const_iterator, keyvalue_pair> const_reverse_iterator;
	//typedef std::reverse_bidirectional_iterator<iterator, keyvalue_pair> reverse_iterator;
	typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
	typedef std::reverse_iterator<iterator> reverse_iterator;
public:
	value_reference operator[](const key_type& k)
	{
		size_t kid = _bucket_index(k, buckets_.size());
		const_iterator iter_find = _bucket_Locate(kid, k);
		if (iter_find == this->end())
		{
			// need to insert a new value ?
			++ count_;
			value_type tmpv;
			iter_find = _insert(kid, k, tmpv);
		}
		
		return ((typename bucket::bucket_node*)iter_find.iter)->data.second;
	}

	//const_value_reference operator[](const key_type& k) const
	//{
	//	const_iterator iter_find = find(k);
	//	if (iter_find == this->end()) 
	//		throw hash_error("not found key in bucket!");
	//	
	//	return iter_find->second;
	//}

public:
	void insert(const key_type& k, const value_type& v)
	{
		size_t kid = _bucket_index(k, buckets_.size());
		const_iterator iter_find = _bucket_Locate(kid, k);
		if (iter_find != this->end()) 
			throw hash_error("found exisited key in bucket!");
		
		_insert(kid, k, v);
		// dec count of key-value pairs
		++ count_;
	}

	void insert(const keyvalue_pair& kv)
	{
		insert(kv.first, kv.second);
	}

	void remove(const key_type& k)
	{
		const_iterator iter_find = find(k);
		if (iter_find == this->end()) 
			throw hash_error("not found key in bucket!");
		bucket& bk = buckets_[iter_find.kid];

		_remove(bk, iter_find.iter);
		if (!bk.count) _setbit(bitmap_.get(), iter_find.kid, false);
		
		delete iter_find.iter;

		// dec count of key-value pairs
		-- count_;
	}
public:
	iterator find(const key_type& k)
	{
		size_t kid = _bucket_index(k, buckets_.size());
		const_iterator iter_find = _bucket_Locate(kid, k);
		return iterator(iter_find.ref, iter_find.kid, iter_find.iter);
	}

	const_iterator find(const key_type& k) const
	{
		size_t kid = _bucket_index(k, buckets_.size());
		const_iterator iter_find = _bucket_Locate(kid, k);
		return iter_find;
	}
public:
	buckethash& operator=(const buckethash& other)
	{
		if (this == &other) return other;
		copy_from(other);
		return *this;
	}
public:
	iterator begin()
	{
		_Iterator tmp(this, 0, NULL);
		tmp.move_to_first();
		return tmp;
	}

	const_iterator begin() const
	{
		_Const_iterator tmp(this, 0, NULL);
		tmp.move_to_first();
		return tmp;
	}

	iterator end()
	{
		_Iterator tmp(this, buckets_.size(), (typename bucket::bucket_node*)NULL);
		return tmp;
	}

	const_iterator end() const
	{
		return _Const_iterator(this, buckets_.size(), NULL);
	}

	const_reverse_iterator rbegin() const
	{
		return const_reverse_iterator(end());
	}

	const_reverse_iterator rend() const
	{
		return const_reverse_iterator(begin());
	}

	reverse_iterator rbegin()
	{
		return reverse_iterator(end());
	}

	reverse_iterator rend()
	{
		return reverse_iterator(begin());
	}
public:
	// return count of key-value pairs
	size_t size() const
	{
		return count_;
	}

	size_t capacity() const
	{
		return buckets_.size();
	}
public:
	void reserve(size_t _Count)
	{
		if (_Count == 0) throw hash_error("count of bucket can't be zero");
		//buckethash tmp(_Count);
		//alloc_.swap(tmp.alloc_);
		kog::smart_vector<bucket> tmp;
		kog::smart_vector<bitmap_item> tmpbitmap(
			(_Count + sizeof(bitmap_item) - 1) / sizeof(bitmap_item) / 8, 0);
		memset(tmp.get(), 0, tmp.size_in_bytes());
		for (size_t i = 0; i < buckets_.size(); ++ i)
		{
			if (buckets_[i].count == 0) continue;
			typename bucket::bucket_node* p = buckets_[i].first;
			typename bucket::bucket_node* first = p;
			do {
				size_t nid = _bucket_index(p->data.first, _Count);
				_insert(tmp[nid], p);
				if(tmp[nid].count == 1) _setbit(tmpbitmap.get(), nid, true);
			}while (p != first);
		}
		tmp.swap(buckets_);
		tmpbitmap.swap(bitmap_);
	}
public:
	bool has_key(const key_type& k) const
	{
		return find(k) != end();
	}
public:
	void clear()
	{
		for (size_t i = 0; i < buckets_.size(); ++ i)
		{
			bucket& bk = buckets_[i];
			if (bk.count)
			{
				typename bucket::bucket_node* p = bk.first;
				do {
					typename bucket::bucket_node* q = p->next;
					delete p;
					p = q;
				} while (p != bk.first);
			}
		}
		// clear data
		memset(buckets_.get(), 0, buckets_.size_in_bytes());
		// clear bit map
		memset(bitmap_.get(), 0, bitmap_.size_in_bytes());
	}
public:
	// swap content
	void swap(buckethash& other) throw()
	{
		buckets_.swap(other.buckets_);
		bitmap_.swap(other.bitmap_);
		std::swap(count_, other.count_);
		alloc_.swap(other.alloc_);
	}
private:
	void copy_from(const buckethash& other)
	{
		clear();
		buckets_.reset(other.buckets_.size());
		bitmap_ = other.bitmap_;
		for (size_t i = 0; i < buckets_.size(); ++ i)
		{
			buckets_[i].count = other.buckets_[i].count;
			typename bucket::bucket_node* p = other.buckets_[i].first;
			if (p == NULL)
			{
				buckets_[i].first = NULL;
				continue;
			}
			do
			{
				typename bucket::bucket_node* n = new typename bucket::bucket_node(p->data);
				_insert(buckets_[i], n);
				p = p->next;
			}while (p != other.buckets_[i].first);
		}
	}
private:
	// get bucket index of given key
	size_t _bucket_index(const key_type& k, size_t bucket_size) const
	{
		_K2I key2index;
		std::size_t idx = abs(key2index(k)) % bucket_size;

		return idx;
	}

	// search key-value in special bucket
	const_iterator _bucket_Locate(size_t kid, const key_type& k) const
	{
		const bucket& b = buckets_[kid];
		typename bucket::bucket_node* p = b.first;
		_Key_equal key_eq;
		size_t i = 0;
		for (; i < b.count && !key_eq(k, p->data.first); ++ i, p = p->next);
		if (i != b.count) return const_iterator(this, kid, p);
		return end();
	}	

	const_iterator _insert(size_t kid, const key_type& k, const value_type& v)
	{
		bucket& bk = buckets_[kid];
		//keyvalue* kv = alloc_.allocate(1);
		typename bucket::bucket_node* n = new typename bucket::bucket_node(k, v);

		// insert into link list
		_insert(bk, n);
		// mark bit map
		_setbit(bitmap_.get(), kid, true);
		return const_iterator(this, kid, n);
	}

	void _insert(bucket& b, typename bucket::bucket_node* n)
	{
		if (b.first == NULL) 
		{
			b.first = n;
			n->next = n->proi = n;
		}
		else 
		{
			n->next = b.first;
			n->proi = b.first->proi;
			n->proi->next = n;
			n->next->proi = n;
		}
		++ b.count;
	}

	typename bucket::bucket_node* _remove(bucket& bk, typename bucket::bucket_node* p)
	{
		if (-- bk.count)
		{
			// remove link from the bucket
			p->proi->next = p->next;
			p->next->proi = p->proi;
		}
		else // bucket because empty
		{
			bk.first = NULL; // just set it NULL
			
		}
		return p;
	}

	void _setbit(bitmap_item* bitmap, size_t kid, bool isSet)
	{
		// clear bit map
		int x = (int)kid / sizeof(bitmap_item) / 8;
		int y = (int)kid - x * sizeof(bitmap_item) * 8;
		if (isSet) bitmap[x] |= 1 << (sizeof(bitmap_item) * 8 - 1 - y);
		else bitmap[x] &= ~(1 << (sizeof(bitmap_item) * 8 - 1 - y));
	}
private:
	std::allocator<typename bucket::bucket_node> alloc_;
	kog::smart_vector<bucket> buckets_;
	kog::smart_vector<bitmap_item> bitmap_; // mark bucket has element(s)
	size_t count_;
};

NAMESPACE_END(kog)

#endif
