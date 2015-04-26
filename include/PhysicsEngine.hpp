#ifndef PHYSICS_ENGINE_HPP
#define PHYSICS_ENGINE_HPP

class RenderList;
class PhysicsNode;

#include<vector>

class Physics{
	public:
		virtual void updatePhysics() = 0;
};
class PhysicsEngine{

	public:
		PhysicsEngine(RenderList* rl);
		virtual ~PhysicsEngine();

		void update();

	private:
	
		RenderList* _rList;
		std::vector< Physics* > _phys_vec;

};


#endif // PHYSICS_ENGINE_HPP
