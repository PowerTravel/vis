#include "RenderList.hpp"
#include "Group.hpp"
#include <iostream>

RenderList::RenderList()
{
	_it = _list.begin();
}

RenderList::~RenderList()
{

}

void RenderList::reset()
{
	_list.clear();
	_list.push_back(RenderNode());
	_it = _list.begin();
	//std::cout << "RL reset: ";
	//_it ->printID();
}

void RenderList::build(Node* node)
{
	reset();
	traverse(node);
	_it = _list.begin();
}

RenderNode* RenderList::get()
{
	if( (_it == _list.end()) || _list.empty() )
	{
		return NULL;
	}
	RenderNode& r = *_it;
	return &r;
}

void RenderList::traverse(Node* node)
{
	if(node->getType() ==  Node::GROUP  )
	{
		Group* grpPtr =(Group*) node;
		
		grow(grpPtr -> getNrChildren());
		
		grpPtr->firstChild();
		Node* childNode = NULL;
		while( (childNode = grpPtr->getChild() ) != NULL )
		{
			traverse( childNode );
			grpPtr->nextChild();
			grpPtr->nextParent();
		}
	}else{
		_it++;	
	}
	node->clean(Node::REBUILD);
}

void RenderList::grow(int n)
{
	if(n>1)
	{
		auto lit = _it;
		lit++;
		_list.insert(lit, n-1, *_it);
		
		lit = _it;
		for(int i = 0; i<n; i++)
		{
			lit->pushID(i);
			lit++;
		}
	}
}


void RenderList::draw()
{
	for(auto it = _list.begin(); it != _list.end(); it++)
	{
		it->draw();
	}
};
		
void RenderList::begin()
{
	_it = _list.begin();
}

void RenderList::next()
{
	_it++;
}

int RenderList::size()
{
	return _list.size();
}

bool RenderList::isEnd()
{
	return (_it == _list.end());
}

void RenderList::print()
{
	for(auto it = _list.begin(); it!=_list.end(); it++)
	{
		it->printID();
		State* st = it->getState();
		st->print();
	}
}

