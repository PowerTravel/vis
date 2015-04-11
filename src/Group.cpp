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
	nodePtr->parentList.push_back(this);
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

void Group::clean()
{	
	if(_dirty)
	{
//		std::cout << "Cleaning Group" << std::endl;
		updateBoundingBox();	
	}
	_dirty = false;
}

void Group::updateBoundingBox()
{
	int nrBoxes = childList.size();
	int nrVertPerBox = 8;
	int nrFloatPerVert = 3;
	int stride = nrVertPerBox * nrFloatPerVert;
	double* points = new double[nrBoxes*stride];
	int i = 0;
	for(NodeList::iterator it = childList.begin(); it!=childList.end(); it++)
	{
		(*it)->getBoundingBoxCorners(&points[i*stride]);
/*
	for(int i = 0; i<8; i++)
		{
			//std::cout << corners[i] << std::endl;
			std::cout << points[3*i + 0] << ", " << points[3*i + 1] << ", " <<  points[3*i + 2]   << std::endl;
		}
		std::cout << "==========" << std::endl;
*/	
		i++;
	}
	_bb = BoundingBox(nrVertPerBox * nrBoxes, points);

	delete [] points;
}
