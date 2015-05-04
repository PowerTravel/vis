#include "Transform.hpp"
#include "NodeVisitor.hpp"

Transform::Transform()
{
	 _m=mat4(1.f);
}

Transform::~Transform()
{

}

mat4 Transform::get()
{
	return _m; 
}

void Transform::set(mat4 m)
{
	_m = m; 
}

void Transform::translate(vec3 ds)
{
	mat4 T = mat4(1.0);
	T = matlib::translate(T, ds);
	_m = T*_m;	
}

void Transform::rotate(float angle, vec3 axis)
{
	vec3 tto = vec3( -_m[3][0], -_m[3][1], -_m[3][2] );
	vec3 tfo = vec3( _m[3][0], _m[3][1], _m[3][2] );
	translate(tto);

	mat4 R = mat4(1.0);

	R = matlib::rotate(R, angle, axis);

	_m = R*_m;
	
	translate(tfo);
}

void Transform::scale(vec3 ds)
{
	mat4 S = mat4(1.0);
	S = matlib::scale(S,ds);
	_m = S*_m;
}

void Transform::acceptVisitor(NodeVisitor& visitor)
{
	visitor.apply(this);
}

