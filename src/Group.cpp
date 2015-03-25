#include "Group.hpp"
#include "NodeVisitor.hpp"
#include <cstdio>
Group::Group()
{
	_type = Node::GROUP;
	childList = std::list<std::shared_ptr<Node> >();
}

Group::~Group()
{
	destroy();
}

void Group::destroy()
{
	while(!childList.empty())
	{
		childList.pop_back();
	}
}

/*
 * Name: 	addChild
 * Purpose:	Adds a child to the node
 * Input:	std::shared_ptr<Node>: The child to be added
 * Output:	-
 * Misc:	-
 */
void Group::addChild(std::shared_ptr<Node> nodePtr )
{
	childList.push_back(nodePtr);
}

/*
 * Name: 	getNrChildren
 * Purpose:	get the number of children the group carries
 * Input:	-
 * Output:	int: the number of children
 * Misc:	-
 */
int Group::getNrChildren()
{
	return childList.size();
}

/*
 * Name: 	acceptVisitor	
 * Purpose:	takes a visitor.
 * Input:	-NodeVisitor&: a reference to the visitor to be accepted
 * Output:	-
 * Misc:	-
 */
void Group::acceptVisitor(NodeVisitor& v)
{	
	v.apply(this);
}
