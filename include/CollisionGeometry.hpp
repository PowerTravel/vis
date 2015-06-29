#ifndef COLLISION_GEOMETRY_HPP
#define COLLISION_GEOMETRY_HPP

#include "Matlib.hpp"
#include "Eigen/Dense"

class CollisionGeometry{

	public:
	
		CollisionGeometry(){};
		virtual ~CollisionGeometry(){};

		virtual bool contain(double x, double y, double z){return false;};
		virtual bool contain(Eigen::Vector3d v){return false;};
		virtual void transform(mat4 t){};
};

class Plane : public CollisionGeometry{
	
	public:
		Plane(){};
		virtual ~Plane(){};

		bool contain(double x, double y, double z){return false;};
		bool contain(Eigen::Vector3d v){return false;};
		void transform(mat4 t){};
};

class Sphere : public CollisionGeometry{
	
	public:
		Sphere(){};
		virtual ~Sphere(){};

		bool contain(double x, double y, double z){return false;};;
		bool contain(Eigen::Vector3d v){return false;};
		void transform(mat4 t){};
};


#endif // COLLISION_GEOMETRY
