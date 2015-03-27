#ifndef	VEC3_HPP
#define VEC3_HPP
#include <iostream>

class Vec4;

// Homogeneous vector
class Vec3
{
	// Input  /  Output streams
	friend std::istream& operator>>(std::istream& is, Vec3& v3);
	friend std::ostream& operator<<(std::ostream& os, const Vec3& v3);
	
	// Dot Product
	friend float operator*(Vec3 v1, Vec3 v2);

	// Cross Product
	friend Vec3 operator^(Vec3 v1, Vec3 v2);

	// Vector Element arithmatic	
	friend Vec3 operator+(Vec3& v1, Vec3& v2);
	friend Vec3 operator-(Vec3& v1, Vec3& v2);
	// Scalar Vector arithmatic
	friend Vec3 operator*(float& f, Vec3& v);
	friend Vec3 operator*(Vec3& v, float f);
	

	public:
		// Flags for what type of norm should be calculated.
		enum NormType{
			L0,
			L1,
			L2,
			INF
		};

		// Constructor / Destructor
		Vec3();
		Vec3(float x, float y, float z);
		virtual ~Vec3();

		// Subscript
		float& operator[](int idx);
		const float& operator[](int idx) const;
		
		// Equality
		bool operator==(const Vec3& v3 );
		bool operator!=(const Vec3& v3 );

		// L0, L1, L2 and INF norm
		float norm(NormType type = L2);
		void normalize();

		// Compound Element arithmatic
		Vec3& operator+=( Vec3& v3 );
		Vec3& operator-=( Vec3& v3 );

		// Compound scalar miultiplication	
		Vec3& operator*=( float& f );
	
		void get(float* r);
		Vec4 asVec4(float w = 0);
	private:
		float v[3];
};

#endif // VEC3_HPP

