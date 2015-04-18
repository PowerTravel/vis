#include <iostream>

#include "GlobalUpdateVisitor.hpp"
#include "Transform.hpp"
#include "Camera.hpp"
#include "Geometry.hpp"
#include "ParticleSystem.hpp"

GlobalUpdateVisitor::GlobalUpdateVisitor(std::shared_ptr<RenderList> rl)
{
	_rList = rl;
	_recreate = true;
}
GlobalUpdateVisitor::~GlobalUpdateVisitor()
{

}

void GlobalUpdateVisitor::init(Group* grp)
{
	NodeVisitor::init(grp);

	std::list<RenderNode>& rl =  _rList->list;
	if(rl.empty() || (grp->getDirtyFlag() & Node::RESET) == Node::RESET )
	{
		_recreate = true;
		rl = std::list<RenderNode>();
		rl.push_back(RenderNode());
		_rit = rl.begin();
		std::cout << "GUV:init"  <<std::endl;
	}
	std::cout << "GUV:init"  <<std::endl;
	_rit = rl.begin();
}

void GlobalUpdateVisitor::reset(Group* grp)
{
	NodeVisitor::reset(grp);
	//_recreate = false;
	std::cout << "GUV:reset" <<std::endl;
}

void GlobalUpdateVisitor::apply(ParticleSystem* n)
{
	_rit->setGeometry(n);

	//State* st = NULL;
	//int flag =  n->getDirtyFlag();
	//if( (flag & (Node::RESET | Node::STATE) ) != Node::CLEAN )
//	{
//		st = n->getState();
//	}
	modify_rList(n->getDirtyFlag(), 0, NULL,NULL,NULL, n->getState());
//	n->clean();
	//printParentChain(n);
}

void GlobalUpdateVisitor::apply(Geometry* n)
{
	_rit->setGeometry(n);
 	
	//State* st = NULL;
	//int flag =  n->getDirtyFlag();
	//if( (flag & (Node::RESET | Node::STATE) ) != Node::CLEAN )
	//{
	//	st = n->getState();
	//}
	modify_rList(n->getDirtyFlag(), 0, NULL, NULL, NULL, n->getState());
	
//	printParentChain(n);
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
	//int flag =  n->getDirtyFlag();
	//if( (flag & (Node::RESET | Node::STATE) ) != Node::CLEAN )
	//{
		modify_rList(n->getDirtyFlag(),  n->getNrChildren(), NULL, NULL, NULL, n->getState());
	//}
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
	//int flag =  n->getDirtyFlag();
	//if( ( flag & (Node::CAM | Node::STATE | Node::RESET) )!=Node::CLEAN )
	//{
		mat4 v = n->getViewMat();
		mat4 p = n->getProjectionMat();
		modify_rList(n->getDirtyFlag(), n->getNrChildren(), NULL, &v, &p, n->getState());
	//}
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
	
	//int flag =  n->getDirtyFlag();
	//if( ( flag & (Node::TRANSFORM | Node::STATE | Node::RESET) )!=Node::CLEAN )
	//{
		modify_rList(n->getDirtyFlag(), n->getNrChildren(), &m, NULL, NULL, n->getState());
	//}
}

void GlobalUpdateVisitor::modify_rList(int flag, int count, mat4* m, mat4* v, mat4* p, State* s)
{
	// Update the current RenderNode
	if( (flag & Node::TRANSFORM) &&  (m != NULL) )
	{
		_rit->setM( *m );
	}
	if( (flag & Node::CAM) &&  (v != NULL) )
	{
		_rit->setV(*v);	
	}
	if( (flag & Node::CAM) && (p != NULL) )
	{
		_rit->setP(*p);	
	}
	if( (flag & Node::STATE) && (s != NULL) )
	{
		_rit->setState(s);
	}
	
	// If the node has more than one child we make count-1 copies of the current RenderNode
	if( (count > 1) && _recreate)
	{
		std::cout << "GUV: Creating " << count-1 << " Nodes" << std::endl;
		std::list<RenderNode>::iterator it = _rit;
		it++;
		_rList->list.insert(it, count-1, *_rit );
	}

	// If we are adding a leaf (aka count == 0), Advance the iterator
	if( (count == 0) && (_rit != _rList->list.end()) )
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

