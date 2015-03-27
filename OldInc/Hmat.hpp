#ifndef HMAT_HPP
#define HMAT_HPP
#include <iostream>
#include <vector>
#include "Vec4.hpp"
#include "Vec3.hpp"

class Hmat
{
	// Input  /  Output streams
	friend std::istream& operator>>(std::istream& is, Hmat& hm);
	friend std::ostream& operator<<(std::ostream& os,const Hmat& hm);

	// Matrix-Matrix Adition, Subtraction and multiplication;	
	friend Hmat operator+(Hmat& m1, Hmat& m2);
	friend Hmat operator-(Hmat& m1, Hmat& m2);
	friend Hmat operator*(Hmat& m1, Hmat& m2);
	
	// Vector-Matrix multiplication
	friend Vec4 operator*(Vec4& v4, Hmat& m);
	friend Vec4 operator*(Hmat& m, Vec4& v4);


	// Vector-Matrix multiplication
		// Vec3 gets implicitly treated as a Vec4 Vector ie w-component is 0
	friend Vec3 operator*(Vec3& v3, Hmat& m);
		// Vec3 gets implicitly treated as a Vec4 Point ie w-component is 1
	friend Vec3 operator*(Hmat& m, Vec3& v3); 

	// Scalar-Matrix multiplication
	friend Hmat operator*(float& f, Hmat& hm);
	friend Hmat operator*(Hmat& hm, float& f);

	public:
		// What type of norm one wants.
		enum NormFlag
		{
			P1,
			INF,
		};
		// How the matrix should be initialized upon creation.
		enum TypeFlag
		{
			IDENTITY,
			ZERO
		};

		// Constructor / Destructors
		Hmat(TypeFlag = IDENTITY);
		Hmat(float mat[]);
		Hmat(Vec4 r0, Vec4 r1, Vec4 r2, Vec4 r3);
		Hmat(Vec3 r0, Vec3 r1, Vec3 r2, Vec3 r3);
		virtual ~Hmat();
		
		void get(float mat[]);

		// Subscript
		Vec4& operator[](int idx);
		const Vec4& operator[](int idx) const;
		// Equality
		bool operator==(const Hmat& hm);	
		bool operator!=(const Hmat& hm);

		// Matrix-Matrix aggregate Adition, Subtraction and multiplication;
		Hmat& operator+=(Hmat& hm);
		Hmat& operator-=(Hmat& hm);
		Hmat& operator*=(Hmat& hm);

		// Scalar-Matrix aggregate multiplication
		Hmat& operator*=(float& f);

		float getVal(int idx); // Get a value using single int, row oriented
		Hmat T(); 			// Get a transposed matrix
		Vec4 row(int idx);	// Get a row vector
		Vec4 col(int idx);	// Get a column vector 
		float norm(NormFlag type=INF);
		Hmat inv(); // Not implemented, returns inverse; Should return null matrix on failure
	private:
		std::vector<Vec4> m;
};

#endif // HMAT_HPP
