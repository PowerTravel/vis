#ifndef GLOBAL_UPDATE_VISITOR_HPP
#define GLOBAL_UPDATE_VISITOR_HPP

#include "NodeVisitor.hpp"
#include "RenderNode.hpp"

class Camera;
class Transform;
class Group;
class Geometry;
class ParticleSystem;

class GlobalUpdateVisitor : public NodeVisitor
{
	public:
		//GlobalUpdateVisitor(std::shared_ptr<RenderList> rl);
		GlobalUpdateVisitor();
		virtual ~GlobalUpdateVisitor();

		void apply(Camera* n);
		void apply(Transform* n);
		void apply(Group* n);
		void apply(Geometry* n);
		void apply(ParticleSystem* n);

	private:

		void init(Group* grp);

		void modify_rList(int count, mat4* m, mat4* v, mat4* p, State* s);

		// DEBUG 
		void printParentChain(Node* n);
};


#endif // GLOBAL_UPDATE_VISITOR_HPP
