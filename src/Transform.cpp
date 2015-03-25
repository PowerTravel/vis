#include "Transform.hpp"
#include "Hmat.hpp"
#include "NodeVisitor.hpp"
#include "Vec3.hpp"
#include <cstdio>

Transform::Transform()
{
	 _T=TransformMatrix();
}

Transform::~Transform()
{

}

Hmat Transform::getM()
{
	return _T.get(); 
}

void Transform::setM(Hmat m)
{
	_T.set(m); 
}

void Transform::translate(Vec3 ds)
{
	_T.translate(ds);
}

void Transform::rotate(float angle, Vec3 axis)
{
	_T.rotate(angle, axis);
}

void Transform::scale(Vec3 ds)
{
	_T.scale(ds);
}

void Transform::acceptVisitor(NodeVisitor& visitor)
{
	visitor.apply(this);
}
