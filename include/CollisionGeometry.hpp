#ifndef COLLISION_GEOMETRY_HPP
#define COLLISION_GEOMETRY_HPP

#include "Matlib.hpp"
#include "Eigen/Dense"
#include <vector>
#include <iostream>


#ifndef COLLISION_GEOMETRY_PTR
#define COLLISION_GEOMETRY_PTR
class CollisionGeometry;
typedef std::shared_ptr<CollisionGeometry> collGeom_ptr;
#endif // COLLISION_GEOMETRY_PTR

// Defined in their separate files
class CollisionPlane; 
class BoundingSphere;
class BoundingBox;

class CollisionGeometry{

	public:

		CollisionGeometry(){ tol = 0.1; };
		virtual ~CollisionGeometry(){};

		virtual void transform(mat4 t) = 0;

		// Point
		virtual bool intersect(Eigen::Vector3d p) = 0;
		// Plane
		virtual bool intersect(CollisionPlane* cp) = 0;
		// Sphere
		virtual bool intersect(BoundingSphere* bs) = 0;

	private:
		// Tolerance for resting contact
		double tol;
};

#endif // COLLISION_GEOMETRY_HPP
