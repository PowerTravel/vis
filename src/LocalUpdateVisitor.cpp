#include "LocalUpdateVisitor.hpp"
#include "Transform.hpp"
#include "Camera.hpp"
#include "Geometry.hpp"
#include "ParticleSystem.hpp"

#include <iostream>

LocalUpdateVisitor::LocalUpdateVisitor()
{

}

void LocalUpdateVisitor::init()
{

}
void LocalUpdateVisitor::cleanup()
{
	_windowChanged = false;
}

LocalUpdateVisitor::~LocalUpdateVisitor(){}

void LocalUpdateVisitor::apply(Geometry* n)
{
	n->update();
}

void LocalUpdateVisitor::apply(ParticleSystem* n)
{
	n->update();
}

void LocalUpdateVisitor::apply(Group* n)
{
	//std::cout << "LocalUpdateVisitor nrParents:" << n->getNrParents() << std::endl;
	n->update();
}

void LocalUpdateVisitor::apply(Transform* n)
{
	n->update();
}

void LocalUpdateVisitor::apply(Camera* n)
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
