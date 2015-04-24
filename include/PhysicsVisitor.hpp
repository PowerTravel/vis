#ifndef PHYSICS_VISITOR_HPP
#define PHYSICS_VISITOR_HPP

#include "NodeVisitor.hpp"

class PhysicsVisitor : public NodeVisitor{
	
	public:
		PhysicsVisitor();
		virtual ~PhysicsVisitor();
	private:
		
		void apply(ParticleSystem* n);
		

};

#endif // PHYSICS_VISITOR_HPP
