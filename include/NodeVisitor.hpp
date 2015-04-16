#ifndef NODEVISITOR_HPP
#define NODEVISITOR_HPP

#include "Node.hpp"

class Node;
class Geometry;
class Group;
class Transform;
class Camera;
class ParticleSystem;
class RenderNode;
//class RenderToTexture;

/*	
 *	Class: 		NodeVisitor
 *	Purpose: 	BaseClass for the visitor classes. 
 *	Misc:		All other visitors inherit from this one.
 */
class NodeVisitor{

	public:

		NodeVisitor();
		virtual ~NodeVisitor();

		// A method for traversing the scene graph
		virtual void traverse( Group* node );

		// Each node has their own apply function
		virtual void apply(Geometry* n);
		virtual void apply(RenderNode* n);
		virtual void apply(Group* n);
		virtual void apply(Transform* n);
		virtual void apply(Camera* n);
		virtual void apply(ParticleSystem* n);
//		virtual void apply(RenderToTexture* n);
		
	protected:
//		std::list<Node*> _nList;

		virtual void init(Group* node);
		virtual void reset(Group* node);
	private:
		void doTraverse(Node* node);
};


#endif // NODEVISITOR_HPP
