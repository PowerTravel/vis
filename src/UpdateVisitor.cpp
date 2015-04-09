#include "UpdateVisitor.hpp"
#include "Transform.hpp"
#include "Camera.hpp"
#include "Geometry.hpp"
#include "ParticleSystem.hpp"

UpdateVisitor::UpdateVisitor()
{

}

void UpdateVisitor::init()
{

}
void UpdateVisitor::cleanup()
{
	_windowChanged = false;
}

UpdateVisitor::~UpdateVisitor(){}

void UpdateVisitor::apply(Geometry* n)
{
	n->update();
}

void UpdateVisitor::apply(ParticleSystem* n)
{
	n->update();
}

void UpdateVisitor::apply(Group* n)
{
	n->update();
}

void UpdateVisitor::apply(Transform* n)
{
	n->update();
}

void UpdateVisitor::apply(Camera* n)
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
