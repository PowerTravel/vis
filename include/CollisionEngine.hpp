#ifndef COLLISION_ENGINE_HPP
#define COLLISION_ENGINE_HPP

//#include "CollisionGeometry.hpp"
#include "BoundingBox.hpp"
#include <vector>
#include <map>
#include <RenderList.hpp>



class CollisionEngine{

	public:
		struct CollisionSet{
			bool collision;
			bool elastic;
			bool collisionPoint[3];
			bool penetrationDepth[3];
		};

		CollisionEngine(RenderList* rl);
		virtual ~CollisionEngine();

		void update();
		
		// Get a list of indices for intersecting particles
	//	void get(int n, const double* x, int& N,  int* ret);

		void get(int n, const double* x, int& N,  int* ret);
		void PrintBoxes();
	private:

		struct container{
			double x[3];
			std::vector<BoundingBox > bb_vec;
		};

		int _max_size; 		// Them max nr of containers in a direction
		double _grid_size;	// The size of a containers
		RenderList* _rList;
		// The key is x + y * _max_size + z * _max_size^2
		// This gives a unique key for every point on the grid map
		std::map<int, container> _grid;

		int getKey(double x, double y, double z);
		void addBox(int key, BoundingBox b);

		// Point Plane Collision
		int contain(Eigen::Vector3d* p, CollisionPlane* cp);
		int contain(CollisionPlane* cp, Eigen::Vector3d* p);

		// Point Spehere Collision
		int contain(Eigen::Vector3d* p, BoundingSphere* s);
		int contain(BoundingSphere* s, Eigen::Vector3d* p);

		// Sphere Plane Collision
		int contain(CollisionPlane* p, BoundingSphere* s);
		int contain(BoundingSphere* s, CollisionPlane* p);

		CollisionSet getCollisionSet(double x, double y, double z, double vx, double vy, double vz);
		
};

#endif // COLLISION_ENGINE_HPP
