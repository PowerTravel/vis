#ifndef BOUNDING_SPHERE_HPP
#define BOUNDING_SPHERE_HPP

#include "CollisionGeometry.hpp"

class BoundingSphere : public CollisionGeometry{
	friend class CollisionPlane;	
	public:

		BoundingSphere();

		void transform(mat4 t){};
	
		// Point
		bool intersect(Eigen::Vector3d p);
		// Plane
		bool intersect(CollisionPlane* cp);
		// Sphere
		bool intersect(BoundingSphere* bs);

	private:
		Eigen::Vector3d _o; // Origin
		double _r; // Radious
};

#endif // BOUNDING_SPHERE_HPP
