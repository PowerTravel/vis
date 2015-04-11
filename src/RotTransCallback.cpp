#include "RotTransCallback.hpp"


RotTransCallback::RotTransCallback(transform_ptr trns, float angle, vec3 axis)
{
	_trns = trns;
	_angle = angle;
	_axis = axis;
}

RotTransCallback::~RotTransCallback()
{
	
}

void RotTransCallback::execute()
{
	_trns->rotate(_angle, _axis);
}
