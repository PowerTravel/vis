#include "Group.hpp"
#include "NodeVisitor.hpp"
#include <iostream>

Group::Group()
{
	_type = Node::GROUP;
	
	_childList = NodeList();
	_cit = _childList.begin();

}

Group::~Group()
{
	destroy();
}

void Group::destroy()
{
	while(!_childList.empty())
	{
		_childList.pop_back();
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
	_childList.push_back(nodePtr);
	
	// if this was the first element
	if(_childList.size() == 1)
	{
		// We set the iterator to point at it
		_cit = _childList.begin();
	}
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
	return _childList.size();
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


void Group::updateBoundingBox()
{
	int nrBoxes = _childList.size();
	int nrVertPerBox = 8;
	int nrFloatPerVert = 3;
	int stride = nrVertPerBox * nrFloatPerVert;
	
	double* points = new double[nrBoxes*stride];
	int i = 0;
	for(NodeList::iterator it = _childList.begin(); it!=_childList.end(); it++)
	{
//		(*it)->getBoundingBoxCorners(&points[i*stride]);
		i++;
	}
//	(*_glit).bb = BoundingBox(nrVertPerBox * nrBoxes, points);
	delete [] points;
}

Node* Group::getChild()
{
	if( _childList.empty() || (_cit == _childList.end()) )
	{
		return NULL;
	}
	return (*_cit).get();
}

void Group::nextChild()
{
	if( _cit != _childList.end() )
	{
		_cit++;
	}
}

void Group::firstChild()
{
	_cit = _childList.begin();
}


void Group::reset()
{ 
	_cit = _childList.begin();
}
