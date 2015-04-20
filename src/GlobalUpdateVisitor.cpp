#include <iostream>

#include "GlobalUpdateVisitor.hpp"
#include "Transform.hpp"
#include "Camera.hpp"
#include "Geometry.hpp"
#include "ParticleSystem.hpp"

/*
GlobalUpdateVisitor::GlobalUpdateVisitor(std::shared_ptr<RenderList> rl)
{
	_rList = rl;
}
*/
GlobalUpdateVisitor::GlobalUpdateVisitor()
{
}
GlobalUpdateVisitor::~GlobalUpdateVisitor()
{

}

void GlobalUpdateVisitor::init(Group* grp)
{
	NodeVisitor::init(grp);

	std::list<RenderNode>& rl =  _rList.list;
	rl = std::list<RenderNode>();
	rl.push_back(RenderNode());
	_rit = rl.begin();
}

void GlobalUpdateVisitor::apply(ParticleSystem* n)
{
	_rit->setGeometry(n);

	modify_rList( 0, NULL,NULL,NULL, n->getState());
}

void GlobalUpdateVisitor::apply(Geometry* n)
{
	_rit->setGeometry(n);
 	
	modify_rList( 0, NULL, NULL, NULL, n->getState());
	
}

/*
 * Name:	Apply (Group)	
 * Purpose: Add entries to the aggregate lists. 
 * Input:	Group* grp: The group to be visited
 * Output:	-
 * Misc:	-
 */
void GlobalUpdateVisitor::apply(Group* n)
{
	modify_rList(  n->getNrChildren(), NULL, NULL, NULL, n->getState());
}

/*
 * Name:	apply (Camera)
 * Purpose: Adds entries to the aggregate lists
 * Input:	Camera* cam: The camera to be visited
 * Output:	-	
 * Misc:	Assumes we only have one camera. The latest camera
 *			is the only one that will be rendered.
 */
void GlobalUpdateVisitor::apply(Camera* n)
{
	mat4 v = n->getViewMat();
	mat4 p = n->getProjectionMat();
	modify_rList( n->getNrChildren(), NULL, &v, &p, n->getState());
}


/*
 * Name:	apply (Transform)	
 * Purpose:	adds its transformationMatrix to the mList as well as
 *			its state.
 * Input:	Transform* t: The transformation node to be visited.	
 * Output:	-	
 * Misc:	-
 */
void GlobalUpdateVisitor::apply(Transform* n)
{	
	mat4 m = n->get();
	
	modify_rList( n->getNrChildren(), &m, NULL, NULL, n->getState());
	
}

void GlobalUpdateVisitor::modify_rList( int count, mat4* m, mat4* v, mat4* p, State* s)
{
	// Update the current RenderNode
	if( m != NULL )
	{
		_rit->setM( *m );
	}
	if( v != NULL )
	{
		_rit->setV(*v);	
	}
	if( p != NULL )
	{
		_rit->setP(*p);	
	}
	if( s != NULL )
	{
		_rit->setState(s);
	}
	
	// If the node has more than one child we make count-1 copies of the current RenderNode
	if( count > 1 )
	{
		std::list<RenderNode>::iterator it = _rit;
		it++;
		_rList.list.insert(it, count-1, *_rit );
	}

	// If we are adding a leaf (aka count == 0), Advance the iterator
	if( (count == 0) && (_rit != _rList.list.end()) )
	{
		_rit++;
	}
}

void GlobalUpdateVisitor::printParentChain(Node* n)
{
	Group* grp = n->getParent();
	while( grp != NULL)
	{
		std::cout << "GotParent" << std::endl;
		grp = grp->getParent();
	}
}

