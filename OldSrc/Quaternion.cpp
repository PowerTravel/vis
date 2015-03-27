#include "Quaternion.hpp"
#include "Vec3.hpp"
#include "Hmat.hpp"
#include <cmath>

#define ROTATE_ZERO_AXIS_EPSILON (float)1E-5

Quaternion::Quaternion()
{
	_v = Vec4(0,0,0,1);
}

Quaternion::Quaternion(float x, float y, float z, float w)
{
	set(x,y,z,w);
}

Quaternion::Quaternion(float angle, Vec3 axis)
{
	_v = Vec4(0,0,0,1);
	makeRotate(angle, axis);
}

Quaternion::~Quaternion()
{

}

void Quaternion::set( Vec3& from, Vec3& to)
{
	float fromNorm = from.norm();
	float toNorm = to.norm();

	Vec3 axis1 = from^to;
	float axis1norm = axis1.norm();
	axis1 = axis1 * (1/axis1norm);	

	// If the vectors are paralell
	if(axis1norm < ROTATE_ZERO_AXIS_EPSILON )
	{
		// If the vectors are coincident
		if(from * to > 0)
		{
			_v = Vec4(0.0f,0.0f,0.0f,1.0f);	
		// If the vectors are opossite
		}else{
			Vec3 tmp;
			
			if(fabs(from[0] < fabs(from[1])) )
			{
				if( fabs(from[0]) < fabs(from[2]) )
				{
					tmp = Vec3(1.0f, 0.0f, 0.0f);
				}else{
					tmp = Vec3(0.0f, 0.0f, 1.0f);
				}
			}else if( fabs( from[1]) < fabs(from[2]) ){
				tmp = Vec3(0.0f, 1.0f, 0.0f);
			}else{
				tmp = Vec3(0.0f, 0.0f, 1.0f);
			}

			Vec3 fromd(from[0], from[1], from[2]);

			// Find an orthogonal axis
			Vec3 axis2 = fromd^tmp;
			axis2 = axis2 * (1/axis2.norm());

			_v[0] = axis2[0];
			_v[1] = axis2[1];
			_v[2] = axis2[2];
			_v[3] = 0; // Cos of pi/2 is zero
		}
	}else{
		// This is the usual situation

		float coangle = from * to / (fromNorm * toNorm);

		if(coangle > 1){
			coangle = 1;
		}else if(coangle < -1){
			coangle = -1;
		}
	
		set(coangle,axis1);
	}
}

void Quaternion::set(float x, float y, float z, float w)
{
	_v = Vec4(x,y,z,w);
	float norm = _v.norm();
	_v = _v *(1/norm);
}

void Quaternion::set(Vec4 vec)
{
	_v = vec;
}

void Quaternion::set(float angle, Vec3 axis)
{
	makeRotate(angle, axis);
}

void Quaternion::makeRotate(float angle, Vec3 vec)
{
	const float epsilon = 0.0000001f;

	float length = vec.norm();
	
	if( length < epsilon)
	{
		// ~zero axis, so reset rotation to zero
		*this = Quaternion();
		return;
	}

	float inversenorm = 1/length;
	float coshalfangle = cos(0.5*angle);
	float sinhalfangle = sin(0.5*angle);

	_v[0] = vec[0] * sinhalfangle * inversenorm;
	_v[1] = vec[1] * sinhalfangle * inversenorm;
	_v[2] = vec[2] * sinhalfangle * inversenorm;
	_v[3] = coshalfangle;
}

void Quaternion::makeRotate(float angle, float x, float y, float z)
{
	makeRotate( angle, Vec3(x,y,z));
}

void Quaternion::makeRotate(Vec3 from, Vec3 to)
{
	const float epsilon = 0.0000001f;

	float fromNorm = from.norm();
	float toNorm = to.norm(); 

	float cosangle = from*to / (fromNorm * toNorm);

	// if vectors are close to being coincident.
	if( fabs(cosangle - 1) < epsilon)
	{
		makeRotate(0.0f, 0.0f, 0.0f, 1.0f);

	// if vectors are close to being opposite
	}else if( fabs(cosangle + 1) < epsilon ){
		
		Vec3 tmp;
		if(fabs(from[0]) < fabs(from[1]) )
		{
			if( fabs(from[0]) < fabs(from[2]))
			{
				tmp = Vec3(1.0f, 0.0f, 0.0f);
			}else{
				tmp = Vec3(0.0f, 0.0f, 1.0f);
			}
		}else if(fabs(from[1]) < fabs(from[2])){
			tmp = Vec3(0.0f, 1.0f, 0.0f);
		}else{
			tmp = Vec3(0.0f, 0.0f, 1.0f);
		}	

		Vec3 axis = from^tmp;
		axis = axis * (1/axis.norm());

		_v[0] = axis[0];			
		_v[1] = axis[1];
		_v[2] = axis[2];
		_v[3] = 0;
	}else{
		// This is the usual situtaion
		Vec3 axis = from^to;
		float angle = acos(cosangle);
		makeRotate(angle, axis);
	}
	
}

void Quaternion::getRotate( float& angle, Vec3& vec )
{
    float x,y,z;
    getRotate(angle,x,y,z);
    vec[0]=x;
    vec[1]=y;
    vec[2]=z;
}

void Quaternion::getRotate(float& angle, float& x, float& y, float& z )
{
    float sinhalfangle = sqrt( _v[0]*_v[0] + _v[1]*_v[1] + _v[2]*_v[2] );
	
    angle = 2 * atan2( sinhalfangle, _v[3] );
    if(sinhalfangle)
    {
        x = _v[0] / sinhalfangle;
        y = _v[1] / sinhalfangle;
        z = _v[2] / sinhalfangle;
    }
    else
    {
        x = 0.0;
        y = 0.0;
        z = 1.0;
    }
}

void Quaternion::slerp(float t, Quaternion& from, Quaternion& to)
{
	const double epsilon = 0.00001;
	double omega, cosomega, sinomega, scale_from, scale_to;

	Quaternion quatTo = to;

	cosomega = from.asVec4() * to.asVec4();

	if( cosomega < 0.0)
	{
		cosomega = -cosomega;
		quatTo = to.negative();
	}

	if( (1.0-cosomega) > epsilon )
	{
		omega = acos(cosomega);
		sinomega = sin(omega);

		scale_from = sin((1.0 - t)*omega)/sinomega;
		scale_to = sin(t*omega)/sinomega;
	}else{
		// If the ends of the vectors are close we use linear interpolation
			scale_from= 1.0-t;
			scale_to = t;
	}

	Vec4 fromVec = from.asVec4();
	Vec4 toVec = quatTo.asVec4();

	fromVec *= (float)scale_from;
	toVec *= (float)scale_to;

	_v = fromVec + toVec;
}

Quaternion Quaternion::negative()
{
	return Quaternion(-_v[0], -_v[1], -_v[2], -_v[3]);
}



Vec4 Quaternion::asVec4()
{
	return _v;
}

Hmat Quaternion::asHmat()
{	
	float x,y,z;
	Vec4 r1,r2,r3,r4;

	x = 1-2*(_v[1]*_v[1] + _v[2]*_v[2]);
	y = 2*(_v[0]*_v[1] - _v[2]*_v[3]);
	z = 2*(_v[0]*_v[2] + _v[1]*_v[3]);
	r1 = Vec4(x,y,z,0.0f);

	x = 2*(_v[0]*_v[1] + _v[2]*_v[3]);
	y = 1-2*(_v[0]*_v[0] + _v[2]*_v[2]);
	z = 2*(_v[1]*_v[2] - _v[0]*_v[3]);
	r2 = Vec4(x,y,z,0.0f);

	x = 2*(_v[0]*_v[2] - _v[1]*_v[3]);
	y = 2*(_v[1]*_v[2] + _v[0]*_v[3]);
	z = 1-2*(_v[0]*_v[0] + _v[1]*_v[1]);
	r3 = Vec4(x,y,z,0.0f);

	r4 = Vec4(0.0f,0.0f,0.0f,1.0f);

	return Hmat(r1,r2,r3,r4);

}

bool Quaternion::zeroRotation()
{ 
	return _v[0]==0.0 && _v[1]==0.0 && _v[2]==0.0 && _v[3]==1.0; 
}

