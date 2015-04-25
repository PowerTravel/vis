#ifndef RENDER_LIST_HPP
#define RENDER_LIST_HPP

#include <list>
#include "NodeVisitor.hpp"
#include "RenderNode.hpp"

class RenderList : public NodeVisitor{

	public:
		RenderList();
		virtual ~RenderList();

		void apply(Geometry* n);
		void apply(RenderNode* n);
		void apply(Group* n);
		void apply(Transform* n);
		void apply(Camera* n);
		void apply(ParticleSystem* n);

		void reset();
		void first();
		bool next();
		RenderNode get();
		
		void add_data( mat4* m, mat4* v, mat4* p, State* s, Geometry* g);

		void build(std::vector<Node*>& vec);

		void draw();
	private:

		std::list<RenderNode> _list;
		std::list<RenderNode>::iterator _it;
};

#endif 
