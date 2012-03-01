// treemaker.h: 
//

#ifndef _TREEMAKER_H_SC_
#define _TREEMAKER_H_SC_

#include <tree.h>
#include <basic_types.h>

NAMESPACE_BEGIN(compile)
NAMESPACE_BEGIN(doc)

class make_tree : public kog::tree<int32>
{
protected:
	link new_leaf(int32 v, link parent)
	{
		link r = new tnode;
		r->children = NULL;
		r->nc = 0;
		r->parent = parent;
		r->v = v;
		return r;
	}

	link new_node(link parent = NULL)
	{
		link r = new tnode;
		r->children = new link[256];
		r->nc = 256;
		r->parent = parent;
		r->v = -1;
		memset(r->children, 0, sizeof(link[256]));
		for (int32 i = 0; i < r->nc; ++ i) 
		{
			r->children[i] = new_leaf(-1, r);
		}
		return r;
	}
};

NAMESPACE_END(doc)
NAMESPACE_END(compile)


#endif