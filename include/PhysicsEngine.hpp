#ifndef PHYSICS_ENGINE_HPP
#define PHYSICS_ENGINE_HPP

class RenderList;
class PhysicsNode;

#include<vector>

class PhysicsEngine{

	public:
		PhysicsEngine(RenderList* rl);
		virtual ~PhysicsEngine();

		void update();

	private:
	
		RenderList* _rList;
		std::vector< PhysicsNode* > _node_vec;

};

#endif // PHYSICS_ENGINE_HPP
