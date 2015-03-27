#include "GL/glew.h"
#include "Camera.hpp"
#include "NodeVisitor.hpp"
#include <cmath>

Camera::Camera()
{
	lookAt(vec3(0.f,0.f,1.f),vec3(0.f,0.f,0.f),vec3(0.f, 1.f, 0.f) );
	setPerspectiveProjection();
}

Camera::Camera(vec3 eye, vec3 at, vec3 up)
{
	lookAt(eye,at,up);
	setPerspectiveProjection();
}

Camera::~Camera()
{

}

void Camera::acceptVisitor(NodeVisitor& v)
{
	v.apply(this);
}

void Camera::lookAt(vec3 eye, vec3 at, vec3 up)
{
	_V = matlib::lookAt(eye, at, up);
}

void Camera::translate(vec3 dr)
{
	_V = matlib::translate(_V, dr);	
}

void Camera::rotate(float angle, vec3 axis)
{
	_V = matlib::rotate(_V, angle, axis);	
}

void Camera::setPerspectiveProjection(float fovy, float aspect, float near, float far)
{
	_P = matlib::perspective(fovy, aspect, near, far);

}

void Camera::setOrthographicProjection(float left, float right, float bottom, float top, float near, float far)
{
	_P = matlib::ortho(left, right, bottom, top, near, far);
}

mat4 Camera::getProjectionMat()
{
	return _P;
}

mat4 Camera::getViewMat()
{
	return _V;
}

void Camera::update()
{
	if( _callback!= NULL){
		_callback->execute();
	}
}
