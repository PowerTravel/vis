#include "GraphVisitor.hpp"
#include <iostream>
#include "Group.hpp"

std::shared_ptr<RenderList> GraphVisitor::_rList = std::shared_ptr< RenderList>(new RenderList);

GraphVisitor::GraphVisitor(){}

GraphVisitor::~GraphVisitor(){}

/*
 * Name:	traverse
 * Purpose: Given a node it will traverse its sub-tree depth first 
 *			recursively. For each node it encounters it will call
 *			the nodes "accept visitor" method.
 * Input:	Node* node: the 'root node' to be traversed.	
 * Output:	-	
 * Misc:	-
 */
void GraphVisitor::traverse(Group* node)
{
	init(node);
	doTraverse(node);
	reset(node);
}

void GraphVisitor::init(Group* g)
{
	g->reset();
}

void GraphVisitor::reset(Group* g)
{
	g->reset();
}

void GraphVisitor::doTraverse(Node* node)
{
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
}

void GraphVisitor::apply(Geometry* n)
{
	printf("Visiting Geometry from GraphVisitor \n");
}

void GraphVisitor::apply(RenderNode* n)
{
	printf("Visiting RenderNode from GraphVisitor \n");
}

void GraphVisitor::apply(Group* n)
{
	printf("Visiting Group from GraphVisitor\n");
}

void GraphVisitor::apply(Transform* n)
{
	printf("Visiting Transform from GraphVisitor \n");
}

void GraphVisitor::apply(Camera* n)
{
	printf("Visiting Camera from GraphVisitor \n");
}

void GraphVisitor::apply(ParticleSystem* n)
{
	printf("Visiting ParticleSystem from GraphVisitor \n");
}
/*
void GraphVisitor::apply(RenderToTexture* n)
{
	printf("Visiting RenderToTexture from GraphVisitor \n");
}
*/

RenderList* GraphVisitor::getRenderList()
{
	return _rList.get();
}
