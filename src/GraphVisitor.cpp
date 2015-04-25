#include "GraphVisitor.hpp"
#include <iostream>

#include "Geometry.hpp"
#include "Transform.hpp"
#include "Camera.hpp"
#include "ParticleSystem.hpp"
//#include "Group.hpp"

std::shared_ptr<RenderList> GraphVisitor::_rList = std::shared_ptr< RenderList>(new RenderList);

GraphVisitor::GraphVisitor()
{
	_node_vec = std::vector<Node*>();
}
GraphVisitor::~GraphVisitor(){}

void GraphVisitor::traverse(Group* node)
{
	init(node);
	doTraverse(node);
	reset(node);
}

void GraphVisitor::init(Group* g)
{
	_rList->first();
//	_rList->reset();

	g->reset();
}

void GraphVisitor::reset(Group* g)
{
}

void GraphVisitor::doTraverse(Node* node)
{

	_node_vec.push_back(node);

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
		}
	}else{
		_rList->build( _node_vec );
	}

	_node_vec.pop_back();
}

void GraphVisitor::apply(Geometry* n)
{
	n->update();
}

void GraphVisitor::apply(RenderNode* n)
{
}

void GraphVisitor::apply(Group* n)
{
	n->update();
}

void GraphVisitor::apply(Transform* n)
{
	n->update();
}

void GraphVisitor::apply(Camera* n)
{
	n->update();
}

void GraphVisitor::apply(ParticleSystem* n)
{
	n->update();
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

