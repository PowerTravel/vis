#ifndef COLLISION_ENGINE_HPP
#define COLLISION_ENGINE_HPP

#include <vector>
#include <map>
//#include "NodeVisitor.hpp"
#include <RenderList.hpp>

class CollisionEngine{

	public:
		CollisionEngine(RenderList* rl);
		virtual ~CollisionEngine();

		void update();
		
		// Get a list of indices for intersecting particles
		void get(int n, const double* x, int& N,  int* ret);
		
	private:

		struct container{
			std::vector<BoundingBox > bb_vec;
		};

		int _max_size; 		// Them max nr of containers in a direction
		double _grid_size;	// The size of a containers
		RenderList* _rl;
		// The key is x + y * _max_size + z * _max_size^2
		// This gives a unique key for every point on the grid map
		std::map<int, container> _grid;

		int getKey(double x, double y, double z);
		void addBox(int key, BoundingBox b);



};

#endif // COLLISION_ENGINE_HPP
