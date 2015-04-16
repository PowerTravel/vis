#include "GL/glew.h"
#include "Camera.hpp"
#include "NodeVisitor.hpp"
#include <iostream>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_access.hpp>

Camera::Camera()
{
	_dw = mat4(1.0f);
	_dr = vec3(0.0f,0.0f,0.0f);	
	
	lookAt(vec3(0.0f,0.0f,80.0f), vec3(0.0f,0.0f,0.0f) , vec3(0.f, 1.f, 0.f) );
	setPerspectiveProjection();
}

Camera::Camera(vec3 eye, vec3 at, vec3 up)
{
	_dw = mat4(1.0f);
	_dr = vec3(0.0f,0.0f,0.0f);	
	
	lookAt(eye, at, up);
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
	_pos = eye;
	_V = matlib::lookAt(eye, at, up);
	dirty(CAM);
}

void Camera::translate(vec3 dr)
{
	_dr += dr;
	_moved = true;
	dirty(CAM);
}

void Camera::rotate(float angle, vec3 axis)
{
	_dw = matlib::rotate(_dw, angle, axis);	
	_moved = true;
	dirty(CAM);
}

void Camera::setPerspectiveProjection(float fovy, float aspect, float near, float far)
{
	_P = matlib::perspective(fovy, aspect, near, far);
	dirty(CAM);
}

void Camera::setOrthographicProjection(float left, float right, float bottom, float top, float near, float far)
{
	_P = matlib::ortho(left, right, bottom, top, near, far);
	dirty(CAM);
}

mat4 Camera::getProjectionMat()
{
	return _P;
}

mat4 Camera::getViewMat()
{
	return _V;
}

mat4 Camera::getRigidInv()
{
	mat4 t = mat4(1.0f);
	t[3][0] = -_V[3][0];
	t[3][1] = -_V[3][1];
	t[3][2] = -_V[3][2];
	t[3][3] = 1;
	

	mat4 r = matlib::transpose(_V);;
	r[0][3] = 0;
	r[1][3] = 0;
	r[2][3] = 0;
	r[3][3] = 1;

	return t*r;
}

void Camera::update()
{
	Node::update();
	if(_moved)
	{
		mat4 v_inv = getRigidInv();

		// New eye
		vec4 dt = v_inv * vec4(_dr,0);	
		_pos = _pos+vec3(dt);

		// New at
		vec4 z_CamCoord =_dw[2]; //matlib::row(_dw,2);
		vec3 z_WorldCoord = matlib::normalize(vec3(v_inv*z_CamCoord));
		vec3 look_at = _pos - z_WorldCoord; 
	
		// new up
		vec4 up_CamCoord = _dw[1]; //matlib::row(_dw,1);
		vec3 up_WorldCoord = vec3(v_inv*up_CamCoord);

		lookAt(_pos, look_at, up_WorldCoord);
		
		_dw = mat4(1.0f);
		_dr = vec3();
		_moved=false;
	}
}
