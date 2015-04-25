
#include "RenderList.hpp"
#include "Geometry.hpp"
#include "Transform.hpp"
#include "RenderNode.hpp"
#include "Camera.hpp"
#include "ParticleSystem.hpp"


RenderList::RenderList()
{
	reset();
}
RenderList::~RenderList(){}

void RenderList::reset()
{
	_list = std::list<RenderNode>();
	_it = _list.begin();
}

void RenderList::draw()
{
	for(auto it = _list.begin(); it != _list.end(); it++)
	{
		it->draw();
	}
}

void RenderList::apply(ParticleSystem* n)
{
	add_data( NULL,NULL,NULL, n->getState(), n);
}
void RenderList::apply(Geometry* n)
{
	add_data( NULL,NULL,NULL, n->getState(), n);
}

void RenderList::apply(Group* n)
{
	add_data( NULL, NULL, NULL, n->getState(), NULL);
}

void RenderList::apply(Camera* n)
{
	mat4 v = n->getViewMat();
	mat4 p = n->getProjectionMat();
	add_data(  NULL, &v, &p, n->getState(), NULL);
}

void RenderList::apply(Transform* n)
{
	mat4 m = n->get();
	
	add_data(&m, NULL, NULL, n->getState(), NULL);
}

void RenderList::apply(RenderNode* n)
{

}

void RenderList::first()
{
	_it = _list.begin();
}

void RenderList::add_data( mat4* m, mat4* v, mat4* p, State* s, Geometry* g)
{
	// Update the current RenderNode
	if( m != NULL )
	{
		_it->setM( *m );
	}
	if( v != NULL )
	{
		_it->setV(*v);	
	}
	if( p != NULL )
	{
		_it->setP(*p);	
	}
	if( s != NULL )
	{
		_it->setState(s);
	}
	if( g != NULL )
	{
		_it->setGeometry(g);
	}
}

void RenderList::build(std::vector<Node*>& vec)
{
	if( _list.empty() )
	{
		_list.push_back(RenderNode());
		_it = _list.begin();
	}

	_it-> clear();
	for(int i = 0; i< vec.size(); i++)
	{
		vec[i]->acceptVisitor(*this);	
	}
/*	
	auto it = _it; 
	it++;
	if( it == _list.end()  )
	{
		_list.push_back(RenderNode());	
	}

	_it++;
*/
	if(!next())
	{
		_list.push_back(RenderNode());	
		next();
	}
}

bool RenderList::next()
{
	auto it = _it; 
	it++;
	if( it == _list.end()  )
	{
		return false;
	}

	_it++;
	return true;
}

RenderNode RenderList::get()
{
	return *_it;
}
