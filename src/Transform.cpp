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
	_m = matlib::translate(_m, ds);
	dirty(TRANSFORM);
}

void Transform::rotate(float angle, vec3 axis)
{
	_m = matlib::rotate(_m, angle, axis);
	dirty(TRANSFORM);
}

void Transform::scale(vec3 ds)
{
	_m = matlib::scale(_m,ds);
	dirty(TRANSFORM);
}

void Transform::acceptVisitor(NodeVisitor& visitor)
{
	visitor.apply(this);
}

