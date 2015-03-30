#include "UpdateVisitor.hpp"
#include "Transform.hpp"
#include "Camera.hpp"
#include "Geometry.hpp"

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

void UpdateVisitor::apply(Geometry* g)
{
	g->update();
}

void UpdateVisitor::apply(Group* grp)
{
	grp->update();
}

void UpdateVisitor::apply(Transform* t)
{
	t->update();
}

void UpdateVisitor::apply(Camera* cam)
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
	cam->update();
}
