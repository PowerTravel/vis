#include "Hmat.hpp"
#include <stdexcept>
Hmat::Hmat(TypeFlag type)
{
	m = std::vector<Vec4>(4);
	for(int i=0; i<4; i++)
	{
		m[i]=Vec4();
		if(type == IDENTITY)
		{
			m[i][i] = 1.0f;
		}
	}
}

Hmat::Hmat(float mat[])
{
	m = std::vector<Vec4>(4);
	for(int i = 0; i<4; i++)
	{
		float x,y,z,w;
		x = mat[i*4+0];
		y = mat[i*4+1];
		z = mat[i*4+2];
		w = mat[i*4+3];
		m[i]=Vec4(x,y,z,w);
	}
}

Hmat::Hmat(Vec4 r0, Vec4 r1, Vec4 r2, Vec4 r3)
{
	m = std::vector<Vec4>(4);
	m[0] = r0;
	m[1] = r1;
	m[2] = r2;
	m[3] = r3;
}

Hmat::Hmat(Vec3 r0, Vec3 r1, Vec3 r2, Vec3 r3)
{
	Vec4 r00 = r0.asVec4();
	Vec4 r11 = r1.asVec4();
	Vec4 r22 = r2.asVec4();
	Vec4 r33 = r3.asVec4();
	m = std::vector<Vec4>(4);
	m[0] = r00;
	m[1] = r11;
	m[2] = r22;
	m[3] = r33;
	m[3][3] = 1;
}

Hmat::~Hmat()
{

}

void Hmat::get(float mat[])
{
	for(int i = 0; i<4; i++)
	{
		for(int j = 0; j<4; j++)
		{
			mat[4*i+j] = m[i][j];
		}
	}
}

std::istream& operator>>(std::istream& is, Hmat& hm)
{
	is >> hm.m[0] >> hm.m[1] >> hm.m[2] >> hm.m[3];
	
	// initialize a default Hmat if is is bad
	if(!is){ hm = Hmat(); }

	return is;

}

std::ostream& operator<<(std::ostream& os,const Hmat& hm)
{
	os << hm.m[0] << "\n";
	os << hm.m[1] << "\n";
	os << hm.m[2] << "\n";
	os << hm.m[3];
	return os;
}

Hmat operator+(Hmat& m1, Hmat& m2)
{
	Vec4 r0,r1,r2,r3;
	r0 = m1.m[0] + m2.m[0];
	r1 = m1.m[1] + m2.m[1];
	r2 = m1.m[2] + m2.m[2];
	r3 = m1.m[3] + m2.m[3];
	return Hmat(r0,r1,r2,r3);
}

Hmat operator-(Hmat& m1, Hmat& m2)
{
	Vec4 r0,r1,r2,r3;
	r0 = m1.m[0] - m2.m[0];
	r1 = m1.m[1] - m2.m[1];
	r2 = m1.m[2] - m2.m[2];
	r3 = m1.m[3] - m2.m[3];
	return Hmat(r0,r1,r2,r3);
}

Hmat operator*(Hmat& m1, Hmat& m2)
{
	Hmat ans = Hmat();
	for(int i=0; i<4; i++)
	{
		for(int j=0; j<4; j++)
		{
			ans[i][j] = m1.row(i) * m2.col(j);
		}
	}
	return ans;
}

// Vector matrix multiplicaion
Vec4 operator*(Vec4& v4, Hmat& hm)
{
	float x, y, z, w;
	x = v4*hm.col(0);
	y = v4*hm.col(1);
	z = v4*hm.col(2);
	w = v4*hm.col(3);
	return Vec4(x,y,z,w);
}

// Matrix vector multiplication
Vec4 operator*(Hmat& hm, Vec4& v4)
{
	float x, y, z, w;
	x = hm.row(0)*v4;
	y = hm.row(1)*v4;
	z = hm.row(2)*v4;
	w = hm.row(3)*v4;
	return Vec4(x,y,z,w);
}


// Vector matrix multiplicaion (Vector)
Vec3 operator*(Vec3& v3, Hmat& hm)
{
	Vec4 v4 = Vec4(v3[0], v3[1], v3[2], 0);
	v4 = hm*v4;
	return Vec3(v4[0], v4[1], v4[2]);
}

// Matrix vector multiplication (Point)
Vec3 operator*(Hmat& hm, Vec3& v3)
{
	Vec4 v4 = Vec4(v3[0], v3[1], v3[2], 1);
	v4 = hm*v4;
	return Vec3(v4[0], v4[1], v4[2]);
}

Hmat operator*(float& f, Hmat& hm)
{	
	Hmat ans = Hmat();
	for(int i=0; i<4; i++)
	{
		ans.m[i] =hm.m[i] * f;
	}
	return ans;
}

Hmat operator*(Hmat& hm, float& f)
{
	return f*hm;
}

Vec4& Hmat::operator[](int idx)
{
	if( (idx<0) || (idx>3) )
	{
		throw std::out_of_range("Index out of range");
	}
	return m[idx];
}

const Vec4& Hmat::operator[](int idx) const
{
	if( (idx<0) || (idx>3) )
	{
		throw std::out_of_range("Index out of range");
	}
	return m[idx];
}

bool Hmat::operator==(const Hmat& hm)
{
	for(int i=0; i<4; i++)
	{
		if (m[i] != hm.m[i]) {
			return false;
		}
	}
	return true;
}

bool Hmat::operator!=(const Hmat& hm)
{
	return !(*this==hm);
}

Hmat& Hmat::operator+=(Hmat& hm)
{
	*this = *this + hm;
	return *this;
}

Hmat& Hmat::operator-=(Hmat& hm)
{
	*this = *this - hm;
	return *this;
}

Hmat& Hmat::operator*=(Hmat& hm)
{
	Hmat ans = Hmat();
	for(int i=0; i<4; i++)
	{
		for(int j=0; j<4; j++)
		{
			ans[i][j] = row(i) * hm.col(j);
		}
	}
	*this = ans;
	return *this;
}


Hmat& Hmat::operator*=(float& f)
{
	for (int i = 0; i < 4; i++) 
	{
		m[i]*= f;
	}
	return *this;
}

// Transpose
Hmat Hmat::T()
{
	Vec4 r0, r1, r2, r3;
	r0=col(0);
	r1=col(1);
	r2=col(2);
	r3=col(3);
	return Hmat(r0, r1, r2,r3);
}

Vec4 Hmat::row(int idx)
{
	if( (idx<0) || (idx>3) )
	{
		throw std::out_of_range("Index out of range");
	}
	return m[idx];
}

Vec4 Hmat::col(int idx)
{
	if( (idx<0) || (idx>3) )
	{
		throw std::out_of_range("Index out of range");
	}
	
	float x,y,z,w;
	x = m[0][idx];
	y = m[1][idx];
	z = m[2][idx];
	w = m[3][idx];

	return Vec4(x,y,z,w);
}

float Hmat::norm(NormFlag type)
{
	float ans = 0;
	// Max absolute column sum
	if(type==Hmat::P1)
	{
		float f1 = col(0).norm(Vec4::L1); 
		for(int i=1; i<4; i++)
		{
			float f2 = col(i).norm(Vec4::L1);
			if(f2>f1)
			{
				f1 = f2;
			}
		}
		ans = f1;
	// Max absolute row sum
	}else if(type == Hmat::INF){
		float f1 = row(0).norm(Vec4::L1); 
		for(int i=1; i<4; i++)
		{
			float f2 = row(i).norm(Vec4::L1);
			if(f2>f1)
			{
				f1 = f2;
			}
		}
		ans = f1;
	}else{
		ans = 0;
	}
	return ans;
}


/// Lifted from MESA implementation of GLU library
Hmat Hmat::inv()
{
	Hmat inv = Hmat(ZERO);


	//  0 Done
	inv[0][0] =   (getVal( 5) * getVal(10) * getVal(15)) -
				  (getVal( 5) * getVal(11) * getVal(14)) -
				  (getVal( 9) * getVal( 6) * getVal(15)) +
				  (getVal( 9) * getVal( 7) * getVal(14)) +
				  (getVal(13) * getVal( 6) * getVal(11)) -
				  (getVal(13) * getVal( 7) * getVal(10));
	//  4 Done
	inv[1][0] = - (getVal( 4) * getVal(10) * getVal(15)) +
				  (getVal( 4) * getVal(11) * getVal(14)) +
				  (getVal( 8) * getVal( 6) * getVal(15)) -
				  (getVal( 8) * getVal( 7) * getVal(14)) -
				  (getVal(12) * getVal( 6) * getVal(11)) +
				  (getVal(12) * getVal( 7) * getVal(10));
	//  8 Done
	inv[2][0] =   (getVal( 4) * getVal( 9) * getVal(15)) -
				  (getVal( 4) * getVal(11) * getVal(13)) -
				  (getVal( 8) * getVal( 5) * getVal(15)) +
				  (getVal( 8) * getVal( 7) * getVal(13)) +
				  (getVal(12) * getVal( 5) * getVal(11)) -
				  (getVal(12) * getVal( 7) * getVal( 9));
	// 12 Done     
	inv[3][0] = - (getVal( 4) * getVal( 9) * getVal(14)) +
				  (getVal( 4) * getVal(10) * getVal(13)) +
				  (getVal( 8) * getVal( 5) * getVal(14)) -
				  (getVal( 8) * getVal( 6) * getVal(13)) -
				  (getVal(12) * getVal( 5) * getVal(10)) +
				  (getVal(12) * getVal( 6) * getVal( 9));
	//  1 Done
	inv[0][1] = - (getVal( 1) * getVal(10) * getVal(15)) +
				  (getVal( 1) * getVal(11) * getVal(14)) +
				  (getVal( 9) * getVal( 2) * getVal(15)) -
				  (getVal( 9) * getVal( 3) * getVal(14)) -
				  (getVal(13) * getVal( 2) * getVal(11)) +
				  (getVal(13) * getVal( 3) * getVal(10));
	//  5 Done     
	inv[1][1] =   (getVal( 0) * getVal(10) * getVal(15)) -
				  (getVal( 0) * getVal(11) * getVal(14)) -
				  (getVal( 8) * getVal( 2) * getVal(15)) +
				  (getVal( 8) * getVal( 3) * getVal(14)) +
				  (getVal(12) * getVal( 2) * getVal(11)) -
				  (getVal(12) * getVal( 3) * getVal(10));
	//  9 Done     
	inv[2][1] = - (getVal( 0) * getVal( 9) * getVal(15)) + 
				  (getVal( 0) * getVal(11) * getVal(13)) +
				  (getVal( 8) * getVal( 1) * getVal(15)) -
				  (getVal( 8) * getVal( 3) * getVal(13)) -
				  (getVal(12) * getVal( 1) * getVal(11)) +
				  (getVal(12) * getVal( 3) * getVal( 9));
	// 13 Done     
	inv[3][1] =   (getVal( 0) * getVal( 9) * getVal(14)) -
				  (getVal( 0) * getVal(10) * getVal(13)) -
				  (getVal( 8) * getVal( 1) * getVal(14)) +
				  (getVal( 8) * getVal( 2) * getVal(13)) +
				  (getVal(12) * getVal( 1) * getVal(10)) -
				  (getVal(12) * getVal( 2) * getVal( 9));
	//  2 Dpme
	inv[0][2] =   (getVal( 1) * getVal( 6) * getVal(15)) -
				  (getVal( 1) * getVal( 7) * getVal(14)) -
				  (getVal( 5) * getVal( 2) * getVal(15)) +
				  (getVal( 5) * getVal( 3) * getVal(14)) +
				  (getVal(13) * getVal( 2) * getVal( 7)) -
				  (getVal(13) * getVal( 3) * getVal( 6));
	//  6 Done     
	inv[1][2] = - (getVal( 0) * getVal( 6) * getVal(15)) +
				  (getVal( 0) * getVal( 7) * getVal(14)) +
				  (getVal( 4) * getVal( 2) * getVal(15)) -
				  (getVal( 4) * getVal( 3) * getVal(14)) -
				  (getVal(12) * getVal( 2) * getVal( 7)) +
				  (getVal(12) * getVal( 3) * getVal( 6));
	// 10 Done     
	inv[2][2] =   (getVal( 0) * getVal( 5) * getVal(15)) - 
				  (getVal( 0) * getVal( 7) * getVal(13)) -
				  (getVal( 4) * getVal( 1) * getVal(15)) +
				  (getVal( 4) * getVal( 3) * getVal(13)) +
				  (getVal(12) * getVal( 1) * getVal( 7)) -
				  (getVal(12) * getVal( 3) * getVal( 5));
	// 14 Done     
	inv[3][2] = - (getVal( 0) * getVal( 5) * getVal(14)) +
				  (getVal( 0) * getVal( 6) * getVal(13)) +
				  (getVal( 4) * getVal( 1) * getVal(14)) -
				  (getVal( 4) * getVal( 2) * getVal(13)) -
				  (getVal(12) * getVal( 1) * getVal( 6)) +
				  (getVal(12) * getVal( 2) * getVal( 5));
	//  3 Done     
	inv[0][3] = - (getVal( 1) * getVal( 6) * getVal(11)) +
				  (getVal( 1) * getVal( 7) * getVal(10)) +
				  (getVal( 5) * getVal( 2) * getVal(11)) -
				  (getVal( 5) * getVal( 3) * getVal(10)) -
				  (getVal( 9) * getVal( 2) * getVal( 7)) +
				  (getVal( 9) * getVal( 3) * getVal( 6));
	//  7 Done     
	inv[1][3] =   (getVal( 0) * getVal( 6) * getVal(11)) -
				  (getVal( 0) * getVal( 7) * getVal(10)) -
				  (getVal( 4) * getVal( 2) * getVal(11)) +
				  (getVal( 4) * getVal( 3) * getVal(10)) +
				  (getVal( 8) * getVal( 2) * getVal( 7)) -
				  (getVal( 8) * getVal( 3) * getVal( 6));
	// 11 Done     
	inv[2][3] = - (getVal( 0) * getVal( 5) * getVal(11)) +
				  (getVal( 0) * getVal( 7) * getVal( 9)) +
				  (getVal( 4) * getVal( 1) * getVal(11)) -
				  (getVal( 4) * getVal( 3) * getVal( 9)) -
				  (getVal( 8) * getVal( 1) * getVal( 7)) +
				  (getVal( 8) * getVal( 3) * getVal( 5));
	// 15 Done     
	inv[3][3] =   (getVal( 0) * getVal( 5) * getVal(10)) -
				  (getVal( 0) * getVal( 6) * getVal( 9)) -
				  (getVal( 4) * getVal( 1) * getVal(10)) +
				  (getVal( 4) * getVal( 2) * getVal( 9)) +
				  (getVal( 8) * getVal( 1) * getVal( 6)) -
				  (getVal( 8) * getVal( 2) * getVal( 5));

	float det = getVal(0)*inv.getVal(0)+ getVal(1)*inv.getVal(4)+
				getVal(2)*inv.getVal(8)+ getVal(3)*inv.getVal(12);

	if( det == 0 )
	{
		return Hmat();
	}

	det = 1.0/det;

	inv *= det;

	return inv;
}

float Hmat::getVal(int idx)
{
	switch(idx)
	{
		case 0:
		{
			return m[0][0];
			break;
		}
		case 1:
		{
			return m[0][1];
			break;
		}
		case 2:
		{
			return m[0][2];
			break;
		}
		case 3:
		{
			return m[0][3];
			break;
		}
		case 4:
		{
			return m[1][0];
			break;
		}
		case 5:
		{
			return m[1][1];
			break;
		}
		case 6:
		{
			return m[1][2];
			break;
		}
		case 7:
		{
			return m[1][3];
			break;
		}
		case 8:
		{
			return m[2][0];
			break;
		}
		case 9:
		{
			return m[2][1];
			break;
		}
		case 10:
		{
			return m[2][2];
			break;
		}
		case 11:
		{
			return m[2][3];
			break;
		}
		case 12:
		{
			return m[3][0];
			break;
		}
		case 13:
		{
			return m[3][1];
			break;
		}
		case 14:
		{
			return m[3][2];
			break;
		}
		case 15:
		{
			return m[3][3];
			break;
		}
		default:
		{
			return 0.0;
			break;
		}
	
	}
}
