#ifndef COLLISION_PLANE_HPP
#define COLLISION_PLANE_HPP

#include "CollisionGeometry.hpp"

class CollisionPlane : public CollisionGeometry{
	friend class BoundingSphere;

	public:
		CollisionPlane();
		virtual ~CollisionPlane();

		void transform(mat4 t);

		// Everything below the plane counts as being "Contained"
		// Point
		bool intersect(Eigen::Vector3d p);
		// Plane
		bool intersect(CollisionPlane* cp);
		// Sphere
		bool intersect(BoundingSphere* bs);

	private:

		Eigen::Vector3d point; // Point, World Coordinates
		Eigen::Vector3d planeNormal; // Plane Normal, World Coordinates
};

#endif // COLLISION_PLANE_HPP
