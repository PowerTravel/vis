#ifndef PHYSICS_ENGINE_HPP
#define PHYSICS_ENGINE_HPP

class RenderList;
class CollisionEngine;

#include "PhysicsVisitor.hpp"

class PhysicsEngine : public PhysicsVisitor{

	public:
		PhysicsEngine(RenderList* rl, CollisionEngine* ce);
		virtual ~PhysicsEngine();

		void update();
		
		void apply(ParticleSystem* n);

	private:
	
		RenderList* _rList;
		CollisionEngine* _clEng;
};


#endif // PHYSICS_ENGINE_HPP
