#ifndef	VEC4_HPP
#define VEC4_HPP
#include <iostream>

#include <Vec3.hpp>
// Homogeneous vector
class Vec4
{
	// Input  /  Output streams
	friend std::istream& operator>>(std::istream& is, Vec4& hv);
	friend std::ostream& operator<<(std::ostream& os, const Vec4& hv);
	
	// Dot Product
	friend float operator*(Vec4 v1, Vec4 v2);

	// Vector Element arithmatic	
	friend Vec4 operator+(Vec4& v1, Vec4& v2);
	friend Vec4 operator-(Vec4& v1, Vec4& v2);
	// Scalar Vector arithmatic
	friend Vec4 operator*(float& f, Vec4& v);
	friend Vec4 operator*(Vec4& v, float f);
	

	public:
		// Flags for what type of norm should be calculated.
		enum NormType{
			L0,
			L1,
			L2,
			INF
		};

		// Constructor / Destructor
		Vec4();
		Vec4(float x, float y=0.0, float z=0.0, float w=0.0);
		virtual ~Vec4();

		// Subscript
		float& operator[](int idx);
		const float& operator[](int idx) const;
		
		// Equality
		bool operator==(const Vec4& hv );
		bool operator!=(const Vec4& hv );

		// L0, L1, L2 and INF norm
		float norm(NormType type = L2);

		// Compound Element arithmatic
		Vec4& operator+=( Vec4& hv );
		Vec4& operator-=( Vec4& hv );

		// Compound scalar miultiplication	
		Vec4& operator*=( float f );

		Vec3 asVec3();
		void get(float* r);

	private:
		float v[4];
};

#endif // VEC4_HPP

