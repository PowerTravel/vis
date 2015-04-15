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
		GlobalUpdateVisitor();
		virtual ~GlobalUpdateVisitor();

		void apply(Camera* n);
		void apply(Transform* n);
		void apply(Group* n);
		void apply(Geometry* n);
		void apply(ParticleSystem* n);
	private:

		void init(Group* grp);
		void reset(Group* grp);
		// The struct that carries the data gatherd from all
		// the nodes the GlobalUpdateVisitor visits.
		struct aggregate_data{
			mat4 m;	// The composite model matrix
			state_ptr s;	// The composite state 
			int count;		// The number of unvisited children;
		};

		// A list containing the updated aggregate_data at each node.
		std::list<RenderNode> _rList;
		std::list<RenderNode>::iterator _rit;
		
		void modify_rList(int count, mat4* m, mat4* v, mat4* p, State* s);
		void draw(VirtualRenderNode& n);
};


#endif // GLOBAL_UPDATE_VISITOR_HPP
