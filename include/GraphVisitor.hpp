#ifndef GRAPH_VISITOR_HPP
#define GRAPH_VISITOR_HPP

#include "NodeVisitor.hpp"
#include "RenderList.hpp"
//#include "RenderNode.hpp"
class Node;
class Geometry;
class Group;
class Transform;
class Camera;
class ParticleSystem;
//class RenderNode;
//class RenderToTexture;


/*	
 *	Class: 		NodeVisitor
 *	Purpose: 	BaseClass for the visitor classes. 
 *	Misc:		All other visitors inherit from this one.
 */
class GraphVisitor : public NodeVisitor{

	public:

		GraphVisitor();
		virtual ~GraphVisitor();

		// A method for traversing the scene graph
		virtual void traverse( Group* node );

		// Each node has their own apply function
		void apply(Geometry* n);
		void apply(RenderNode* n);
		void apply(Group* n);
		void apply(Transform* n);
		void apply(Camera* n);
		void apply(ParticleSystem* n);
//		void apply(RenderToTexture* n);

		RenderList* getRenderList();

	protected:
		static std::shared_ptr<RenderList> _rList;
//		std::list<RenderNode>::iterator _rit;

		virtual void init(Group* node);
		virtual void reset(Group* node);
	private:
		std::vector<Node*> _node_vec;

		void doTraverse(Node* node);
};


#endif // GRAPH_VISITOR_HPP
