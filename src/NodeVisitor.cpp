#include "NodeVisitor.hpp"
#include <iostream>
//#include "Geometry.hpp"
#include "Group.hpp"
//#include "Transform.hpp"

NodeVisitor::NodeVisitor()
{
//	_nList = std::list<Node*>();
}

NodeVisitor::~NodeVisitor(){}

/*
 * Name:	traverse
 * Purpose: Given a node it will traverse its sub-tree depth first 
 *			recursively. For each node it encounters it will call
 *			the nodes "accept visitor" method.
 * Input:	Node* node: the 'root node' to be traversed.	
 * Output:	-	
 * Misc:	-
 */
void NodeVisitor::traverse(Group* node)
{
	init(node);
	doTraverse(node);
	reset(node);
}

void NodeVisitor::init(Group* g)
{
	g->reset();
}

void NodeVisitor::reset(Group* g)
{
	g->reset();
}

void NodeVisitor::doTraverse(Node* node)
{
	//_nList.push_front(node);

	// Inject itself into the node
	node->acceptVisitor(*this);

	if(node->getType() ==  Node::GROUP  )
	{
		Group* grpPtr =(Group*) node;
		grpPtr->firstChild();
		Node* childNode = NULL;
		while( (childNode = grpPtr->getChild() ) != NULL )
		{
			doTraverse( childNode );
			grpPtr->nextChild();
			grpPtr->nextParent();
		}
	}

	//_nList.pop_front();
}

void NodeVisitor::apply(Geometry* n)
{
	printf("Visiting Geometry from NodeVisitor \n");
}

void NodeVisitor::apply(RenderNode* n)
{
	printf("Visiting RenderNode from NodeVisitor \n");
}

void NodeVisitor::apply(Group* n)
{
	printf("Visiting Group from NodeVisitor\n");
}

void NodeVisitor::apply(Transform* n)
{
	printf("Visiting Transform from NodeVisitor \n");
}

void NodeVisitor::apply(Camera* n)
{
	printf("Visiting Camera from NodeVisitor \n");
}

void NodeVisitor::apply(ParticleSystem* n)
{
	printf("Visiting ParticleSystem from NodeVisitor \n");
}
/*
void NodeVisitor::apply(RenderToTexture* n)
{
	printf("Visiting RenderToTexture from NodeVisitor \n");
}
*/
