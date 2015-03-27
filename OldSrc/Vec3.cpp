#include "Vec3.hpp"
#include "Vec4.hpp"
//#include "Hmat.h"
#include <cmath>
#include <stdexcept>
Vec3::Vec3()
{
	v[0]=0.0;
	v[1]=0.0;
	v[2]=0.0;
}

Vec3::Vec3(float x, float y, float z)
{
	v[0]=x;
	v[1]=y;
	v[2]=z;
}

Vec3::~Vec3()
{

}


// read
std::istream& operator>>(std::istream& is, Vec3& v3)
{
	is >> v3.v[0] >> v3.v[1] >> v3.v[2];
	
	// initialize a default Vec3 if is is bad
	if(!is){ v3 = Vec3(); }

	return is;
}

// write
std::ostream& operator<<(std::ostream& os, const Vec3& v3)
{
	os << v3[0] << " " << v3[1] << " " << v3[2]; 
	return os;
}

// Cross Product
Vec3 operator^( Vec3 v1, Vec3 v2 )
{
	float x,y,z;
	x = v1[1]*v2[2] - v1[2]*v2[1];
	y = v1[2]*v2[0] - v1[0]*v2[2];
	z = v1[0]*v2[1] - v1[1]*v2[0];
	return Vec3(x,y,z);
}

// Dot product
float operator*( Vec3 v1, Vec3 v2 )
{
	float x,y,z;
	x = v1.v[0] * v2.v[0];
	y = v1.v[1] * v2.v[1];
	z = v1.v[2] * v2.v[2];
	return x+y+z;
}

// vector adition
Vec3 operator+(Vec3& v1, Vec3& v2)
{
	float x,y,z;
	x=v1.v[0]+v2.v[0];
	y=v1.v[1]+v2.v[1];
	z=v1.v[2]+v2.v[2];
	return Vec3(x,y,z);
}

// Vector subtraction
Vec3 operator-(Vec3& v1, Vec3& v2)
{
	float x,y,z;
	x=v1.v[0]-v2.v[0];
	y=v1.v[1]-v2.v[1];
	z=v1.v[2]-v2.v[2];
	return Vec3(x,y,z);
}

// Scalar-Vector Multiplication
Vec3 operator*(float& f, Vec3& v3)
{
	// For some reason this function cant access the private member v
	float x,y,z,w;
	x = f*v3[0];
	y = f*v3[1];
	z = f*v3[2];
	return Vec3(x,y,z);
}

// Scalar-Vector Multiplication
Vec3 operator*(Vec3& v3, float f)
{
	return f*v3;
}

// Subscript
float& Vec3::operator[](int idx)
{
	if( (idx<0) || (idx>2) ){
		throw std::out_of_range("index out of range");
	}
	return v[idx];
}

// Const Subscript
const float& Vec3::operator[](int idx) const
{
	if( (idx<0) || (idx>2) ){
		throw std::out_of_range("index out of range");
	}
	return v[idx];
}

// Equal to
bool Vec3::operator==( const Vec3& v3 )
{
	if( (v[0]==v3[0]) && (v[1]==v3[1]) && (v[2]==v3[2]) )
	{
		return true;
	}else{
		return false;
	}
}


// Not equal to
bool Vec3::operator!=( const Vec3& v3 )
{
	return !( *this == v3 );
}


// L0, L1, L2 and INF norm
float Vec3::norm(NormType type)
{
	float ans=0;
	switch(type)
	{
		case L0:
		{
			float j=0;
			for (int i = 0; i<3; i++)
			{
				if(v[i] != 0)
				{
					j++;
				}
			}
			ans = j;
			break;
		}
		case L1:
		{
			ans = abs(v[0])+abs(v[1])+abs(v[2]);
			break;
		}
		case L2:
		{
			ans = sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
			break;
		}
		case INF:
		{
			float val1  = abs(v[0]);
			for(int i=1; i<3; i++)
			{	
				float val2 = abs(v[i]);
				if(val1 < val2)
				{
					val1 = val2;
				}
			}
			ans = val1;
			break;
		}
		default:
		{
			ans = 0;
			break;
		}
	}
	return ans;
}

void Vec3::normalize()
{
	float invNorm =1 / this->norm();
	v[0] *= invNorm;	
	v[1] *= invNorm;
	v[2] *= invNorm;
}


// Compound vector Adition
Vec3& Vec3::operator+=( Vec3& v3 )
{	
	*this = *this + v3;
	return *this;
}

// Compound Vector subtraction
Vec3& Vec3::operator-=( Vec3& v3 )
{
	*this = *this - v3;
	return *this;
}


// Compound scalar vector multiplication
Vec3& Vec3::operator*=( float& f )
{
	*this = *this * f;
	return *this;
}


void Vec3::get(float* r)
{
	r[0] = v[0];
	r[1] = v[1];
	r[2] = v[2];
}

Vec4 Vec3::asVec4(float w)
{
	return Vec4(v[0],v[1],v[2],w);
}
