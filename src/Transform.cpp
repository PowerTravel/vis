#include "Transform.hpp"
#include "NodeVisitor.hpp"

#include <glm/gtc/matrix_transform.hpp>


Transform::Transform()
{
	 _m=mat4();
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
	_m = glm::translate(_m, ds);
}

void Transform::rotate(float angle, vec3 axis)
{
	_m = glm::rotate(_m, angle, axis);
}

void Transform::scale(vec3 ds)
{
	_m = glm::scale(_m,ds);
}

void Transform::acceptVisitor(NodeVisitor& visitor)
{
	visitor.apply(this);
}
