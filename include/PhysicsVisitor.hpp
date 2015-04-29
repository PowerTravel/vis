#ifndef PHYSICS_VISITOR_HPP 
#define PHYSICS_VISITOR_HPP 

class ParticleSystem;

class PhysicsVisitor{

	public:
	//virtual void apply(Geometry* n) = 0;
	//virtual void apply(RenderNode* n) = 0;
		virtual void apply(ParticleSystem* n) = 0;;


};
#endif  // PHYSICS_VISITOR_HPP
