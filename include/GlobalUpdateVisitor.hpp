#ifndef GLOBAL_UPDATE_VISITOR_HPP
#define GLOBAL_UPDATE_VISITOR_HPP

#include "RenderList.hpp"

class Camera;
class Transform;
class Group;
class Geometry;
class ParticleSystem;

class GlobalUpdateVisitor : public NodeVisitor
{
	public:
		GlobalUpdateVisitor();
		virtual ~GlobalUpdateVisitor();

		void apply(Camera* n);
		void apply(Transform* n);
		void apply(Group* n);
		void apply(Geometry* n);
		void apply(ParticleSystem* n);
	private:
		void accumulateState(Node* n);
}


#endif // GLOBAL_UPDATE_VISITOR_HPP
