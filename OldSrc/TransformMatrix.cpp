#include "TransformMatrix.hpp"
#include "Quaternion.hpp"
#include "Vec3.hpp"

TransformMatrix::TransformMatrix()
{
	_m = Hmat();
}
TransformMatrix::TransformMatrix(Hmat hm)
{
	_m = hm;
}

TransformMatrix::~TransformMatrix()
{

}

void TransformMatrix::rotate(float angle, Vec3 axis)
{
	//Hmat m_inv = getRigidInverse();
	// tto = translate TO origin,
	// tfo = translate (back) FROM origin;
	Vec3 tto = Vec3( -_m[0][3], -_m[1][3], -_m[2][3] );
	Vec3 tfo = Vec3( _m[0][3], _m[1][3], _m[2][3] );
	translate(tto);

	Quaternion q = Quaternion(angle,axis);
	Hmat R = q.asHmat();

	_m = R*_m;

	translate(tfo);

}

void TransformMatrix::rotate(float angle, Vec3 axis, Vec3 p)
{
	// tto = translate TO origin,
	// tfo = translate (back) FROM origin to world coordinates;
//	Vec3 tto = Vec3( -p[0], -p[1], -p[2] );
//	translate(tto);

//	Quaternion q = Quaternion(angle,axis);
//	Hmat R = q.asHmat();

//	_m = R*_m;

//	translate(p);
}

void TransformMatrix::scale(Vec3 scale)
{
	Vec3 p = Vec3(_m[0][3],_m[1][3],_m[2][3]);
	Vec3 tto = Vec3( -p[0], -p[1], -p[2] );
	translate(tto);

	Hmat S = Hmat();
	
	for(int i=0; i<3; i++)
	{
		S[i][i] = scale[i];
	}

	_m = _m*S;
	
	translate(p);
}


Hmat TransformMatrix::getRigidInverse()
{	
	Hmat t_inv = Hmat();
	t_inv[0][3] = -_m[0][3];
	t_inv[1][3] = -_m[1][3];
	t_inv[2][3] = -_m[2][3];
	t_inv[3][3] = 1;

	Hmat r_inv = _m.T();
	r_inv[3][0] = 0;
	r_inv[3][1] = 0;
	r_inv[3][2] = 0;
	r_inv[3][3] = 1;

	return t_inv * r_inv;
}

void TransformMatrix::translate(Vec3 trans)
{
	Hmat T = Hmat();
	T[0][3]=trans[0]; 
	T[1][3]=trans[1];
	T[2][3]=trans[2];
	T[3][3]= 1;
	_m= T*_m;
}

Hmat TransformMatrix::get()
{
	return _m;
}

void TransformMatrix::set(Hmat m)
{
	_m =m;
}
		
		
//Vec3 TransformMatrix::getPosition()
//{
//	return Vec3(_m[0][3],_m[1][3],_m[2][3]);
//}
