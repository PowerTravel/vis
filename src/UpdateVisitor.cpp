#include "UpdateVisitor.hpp"
#include "RenderToTexture.hpp"
#include "Transform.hpp"
#include "Camera.hpp"
#include "Geometry.hpp"
#include "KeyState.hpp"
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

void UpdateVisitor::apply(RenderToTexture* tex)
{
	if(_windowChanged == true)
	{
    	GLint portSize[4];
    	glGetIntegerv( GL_VIEWPORT, portSize );
		float w = (float) portSize[2]-portSize[0];
		float h = (float) portSize[3]-portSize[1];
		
		tex->resizeTexture(w,h);	
	}
	tex->update();
}
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
	if(_windowChanged == true)	
	{
    	GLint portSize[4];
    	glGetIntegerv( GL_VIEWPORT, portSize );
		float w = (float) portSize[2]-portSize[0];
		float h = (float) portSize[3]-portSize[1];
		cam->setAspect(w/h);
		cam->setPerspectiveProjection();
	}
	cam->update();
}
		
void UpdateVisitor::windowChanged()
{
	_windowChanged = true;
}
