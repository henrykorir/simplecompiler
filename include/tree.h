// tree.h: 
//

#ifndef _TREE_H_HUXL_
#define _TREE_H_HUXL_

#include <memory>
#include <stdexcept>

#include <macros.h>

NAMESPACE_BEGIN(kog)

template<typename _Tx, typename _Alloc>
class _tree_node
{
public:
	typedef _Tx value_type;
protected:
	struct tnode
	{
		typedef tnode* link;
		link* children;
		int nc;
		link parent;
		value_type v;
	};

	typedef tnode* link;
	typedef const tnode* const_link;

	_tree_node(_Alloc _Al = _Alloc())
		: Alval_(_Al)
	{
	}

public:
	typedef typename _Alloc::template rebind<tnode>::other _Alnode;
	typedef typename _Alloc::template rebind<link>::other _Allink;

	_Alnode Alval_;
	_Allink Allink_;
};

template<typename _Tx, typename _Alloc = std::allocator<_Tx> >
class tree : public _tree_node<_Tx, _Alloc>
{
public:
	typedef typename mpl::remove_const<_Tx>::type value_type;
	typedef typename mpl::add_const<_Tx>::type const_type;
	typedef _tree_node<_Tx, _Alloc> _Base;
	typedef typename _Base::link link;
	typedef typename _Base::const_link const_link;

public:
	template<typename _Bidirection>
	struct _Const_iterator : public std::iterator<std::bidirectional_iterator_tag, _Tx>
	{
	public:
		typedef std::iterator<std::bidirectional_iterator_tag, _Tx> _Base;
		typedef typename _Base::iterator_category iterator_category;
		typedef typename _Base::value_type value_type;
		typedef typename _Base::difference_type difference_type;
		typedef typename _Base::difference_type distance_type;	// retained
		typedef typename _Base::pointer pointer;
		typedef typename _Base::reference reference;
		typedef const value_type& const_reference;
		typedef const value_type* const_pointer;
		typedef _Const_iterator<_Tx> _Myt;
		typedef typename tree<_Tx, _Alloc>::link link;
	public:
		_Const_iterator(link p)
		: self_(p)
		{}

	public:
		const_pointer operator ->() const { return &self_->v; }
		const_reference operator*() const { return self_->v; }

	public:
		_Myt& operator++() { return forward(*this); }
		_Myt& operator--() { return backward(*this); }
		_Myt operator++(int) { _Myt temp(*this); forward(*this); return temp;}
		_Myt operator--(int) { _Myt temp(*this); backward(*this); return temp; }

	protected:
		void forward(_Myt& iter)
		{
			_Bidirection b(iter.self_);
			++ b;
		}

		void backward(_Myt& iter)
		{
			_Bidirection b(iter.self_);
			-- b;
		}
	protected:
		link self_;
	};

	template<typename _Bidirection>
	struct _Iterator : public _Const_iterator<_Bidirection>
	{
	public:
		typedef _Const_iterator<_Bidirection> _Const_base;
		typedef typename _Base::iterator_category iterator_category;
		typedef typename _Base::value_type value_type;
		typedef typename _Base::difference_type difference_type;
		typedef typename _Base::difference_type distance_type;	// retained
		typedef typename _Base::pointer pointer;
		typedef typename _Base::reference reference;
		typedef _Iterator<_Tx> _Myt;
	public:
		_Iterator(link p)
		: _Const_base(p)
		{
		}

	public:
		pointer operator ->() const { return &_Const_base::self_->v; }
		reference operator*() const { return _Const_base::self_->v; }
	};
	
	struct first_order
	{
		first_order(link p) : p_(p) {}
		
		first_order& operator++() const 
		{
		}
		
		link p_;
	};

public:
	tree()
	: root_(NULL) // empty tree
	{
	}

	~tree()
	{
	}

public:
	link root() { return root_; }
	const_link root() const { return root_; }
protected:
	void reset_root(link r)
	{
		root_ = r;
	}
private:
	link root_;
};

NAMESPACE_END(kog)

#endif
