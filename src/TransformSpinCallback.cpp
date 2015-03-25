#include "TransformSpinCallback.hpp"
#include "Transform.hpp"
#include <cstdio>
TransformSpinCallback::TransformSpinCallback(transform_ptr t)
{
	_t = t;
	_axis = Vec3(0,1,0);
	_angle = 0.01;
}
TransformSpinCallback::TransformSpinCallback(transform_ptr t, float angle, Vec3 axis)
{
	_t = t;
	_axis = axis;
	_angle = angle;
}
TransformSpinCallback::~TransformSpinCallback()
{

}

void TransformSpinCallback::execute()
{
	_t->rotate(_angle, _axis);
}
