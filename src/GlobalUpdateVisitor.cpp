#include "GlobalUpdateVisitor.hpp"
#include "Transform.hpp"
#include "Camera.hpp"
#include "Geometry.hpp"
#include "ParticleSystem.hpp"

GlobalUpdateVisitor::GlobalUpdateVisitor()
{

}

void GlobalUpdateVisitor::init()
{

}

void GlobalUpdateVisitor::cleanup()
{
	_windowChanged = false;
}

GlobalUpdateVisitor::~GlobalUpdateVisitor()
{

}

void GlobalUpdateVisitor::apply(Geometry* n)
{
	n->update();
}

void GlobalUpdateVisitor::apply(ParticleSystem* n)
{
	n->update();
}

void GlobalUpdateVisitor::apply(Group* n)
{
	n->update();
}

void GlobalUpdateVisitor::apply(Transform* n)
{
	n->update();
}

void GlobalUpdateVisitor::apply(Camera* n)
{
/*
	if(_windowChanged == true)	
	{
    	GLint portSize[4];
    	glGetIntegerv( GL_VIEWPORT, portSize );
		float w = (float) portSize[2]-portSize[0];
		float h = (float) portSize[3]-portSize[1];
		cam->setPerspectiveProjection(45,w/h,0.1f, 100);
	}
*/
	n->update();
}
		
void accumulateState(Node* n)
{
	std::list<Node*>& pl = n->parentList;
	
	for(std::list<Node*>::iterator it = pl.begin(); it != pl.end(); ++it)
	{
		()
	}


}
