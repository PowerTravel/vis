#include <iostream>

#include "GlobalUpdateVisitor.hpp"
#include "Transform.hpp"
#include "Camera.hpp"
#include "Geometry.hpp"
#include "ParticleSystem.hpp"

GlobalUpdateVisitor::GlobalUpdateVisitor()
{
	//_recreate = true;
}
GlobalUpdateVisitor::~GlobalUpdateVisitor()
{

}
/*
void GlobalUpdateVisitor::init(Group* grp)
{
	NodeVisitor::init(grp);
}

void GlobalUpdateVisitor::reset(Group* grp)
{
	NodeVisitor::reset(grp);
}
*/
void GlobalUpdateVisitor::apply(ParticleSystem* n)
{
	RenderNode* rn = render_list.get();
	rn->setGeometry(n);
	modify_rList(n->getDirtyFlag(), 0, NULL,NULL,NULL, n->getState());
}

void GlobalUpdateVisitor::apply(Geometry* n)
{
	RenderNode* rn = render_list.get();
	rn->setGeometry(n);
 	
	modify_rList(n->getDirtyFlag(), 0, NULL, NULL, NULL, n->getState());
	
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
	modify_rList(n->getDirtyFlag(),  n->getNrChildren(), NULL, NULL, NULL, n->getState());
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
	modify_rList(n->getDirtyFlag(), n->getNrChildren(), NULL, &v, &p, n->getState());
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
	modify_rList(n->getDirtyFlag(), n->getNrChildren(), &m, NULL, NULL, n->getState());
}

void GlobalUpdateVisitor::modify_rList(int flag, int count, mat4* m, mat4* v, mat4* p, State* s)
{
	RenderNode* rn= render_list.get();

	if( m != NULL )
	{
		rn->setM( *m );
	}
	if( v != NULL )
	{
		rn->setV(*v);	
	}
	if( p != NULL )
	{
		rn->setP(*p);	
	}
	if( s != NULL )
	{
		rn->setState(s);
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
