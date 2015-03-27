#ifndef TRANSFORM_MATRIX_HPP
#define TRANSFORM_MATRIX_HPP
#include "Hmat.hpp"

class Vec3;

class TransformMatrix{

	public:
		TransformMatrix();
		TransformMatrix(Hmat hm);
		virtual ~TransformMatrix();

		// Rotates around its center
		void rotate(float angle, Vec3 axis);
		// Rotates around point p
		void rotate(float angle, Vec3 axis, Vec3 p);
		void scale(Vec3 sacle);
		void translate(Vec3 trans);
		Hmat get();
		void set(Hmat m);

		Hmat getRigidInverse();

//		Vec3 getPosition();

	private:
		Hmat _m;
};

#endif //TRANSFORM_MATRIX_HPP
