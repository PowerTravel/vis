#include "Node.hpp"
#include "Group.hpp"
#include <iostream> // Debug

Node::Node()
{
	_state = State();
	_type = NODE;
	_callback = NULL;	
}

Node::~Node()
{
	_callback = NULL;
}

void Node::update()
{
	if( _callback!= NULL){
		_callback->execute();
	}
}

void Node::connectCallback(callback_ptr cb)
{
	_callback = cb;
}

UpdateCallback* Node::getUpdateCallback()
{
	return _callback.get();
}

void Node::setState(State* s)
{
	if(s != NULL){
		_state.merge(s);
	}
}

State* Node::getState()
{
	return &_state;
}

Node::N_Type Node::getType()
{
	return _type;
}


BoundingBox Node::getBoundingBox()
{
	return _bb;
}

void Node::setBoundingBox(BoundingBox bb)
{
	_bb = bb;
}

CollisionGeometry Node::getCollisionGeometry()
{
	return _cg;
}
void Node::setCollisionGeometry(CollisionGeometry cg)
{
	_cg = cg;
}
