#ifndef NODEVISITOR_HPP
#define NODEVISITOR_HPP

#include "State.hpp"
#include "Node.hpp"

class Node;
class Geometry;
class Group;
class Transform;
//class Camera;
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
		virtual void traverse( Node* node );

		// Each node has their own apply function
		virtual void apply(Geometry* n);
		virtual void apply(Group* n);
		virtual void apply(Transform* n);
//		virtual void apply(Camera* n);
//		virtual void apply(RenderToTexture* n);
		
	protected:
		virtual void init();
		virtual void cleanup();
	private:
		void doTraverse(Node* node);
};


#endif // NODEVISITOR_HPP
