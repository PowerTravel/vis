#include "Group.hpp"
#include "NodeVisitor.hpp"
#include <iostream>
Group::Group()
{
	_type = Node::GROUP;
	_dFlag = STATE & TRANSFORM;
	
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
	nodePtr->addParent(this);
	
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

void Group::dirty(dirty_bit bit)
{
	// Cant accept CLEAN bit
	if(bit == CLEAN)
	{
		return;
	}
	if( (bit & _dFlag) != bit  )
	{
		std::cout << "Group: dirtying " << std::endl;
		std::cout << "	Bit:  " << bit  << std::endl;
		std::cout << "	Flag: " << _dFlag  << std::endl;
		// Bits that exist in both bit and _dFlag
		int same = _dFlag & bit; 
		// Bits that exists in either but not both
		int different = _dFlag ^ bit; 	
		_dFlag = same +  different;
		std::cout << "	Same:  " << same  << std::endl;
		std::cout << "	diff: " << different  << std::endl;
	}
}

void Group::clean()
{	
	_dFlag = CLEAN;
}

Node* Group::getChild()
{
	if(_cit != _childList.end())
	{
		return (*_cit).get();
	}
	return NULL;
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
	//std::cout << "Grp Reset" << std::endl;
	_cit = _childList.begin();
	while(_cit != _childList.end())
	{
		(*_cit)->reset();
		_cit++;
	}
	_cit = _childList.begin();
}
