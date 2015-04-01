#include "NodeVisitor.hpp"
//#include "Geometry.hpp"
#include "Group.hpp"
//#include "Transform.hpp"

NodeVisitor::NodeVisitor(){}

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
void NodeVisitor::traverse(Node* node)
{
	init();
	doTraverse(node);
	cleanup();
}

void NodeVisitor::init()
{
}
void NodeVisitor::cleanup()
{
}

void NodeVisitor::doTraverse(Node* node)
{
	// Inject itself into the node
	node->acceptVisitor(*this);
	
	// If the node is of type GROUP we traverse it's subtree if it
	// is a GROUP
	if(node->getType() ==  Node::GROUP  )
	{
		Group* grpPtr =(Group*) node;
		NodeList childList = grpPtr->childList;
		for(NodeList::iterator ci = childList.begin(); ci != childList.end(); ci++)
		{
			doTraverse(ci->get());
		}
	}

}

void NodeVisitor::apply(Geometry* n)
{
	printf("Visiting Geometry from NodeVisitor \n");
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
