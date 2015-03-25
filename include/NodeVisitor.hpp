#ifndef NODEVISITOR_HPP
#define NODEVISITOR_HPP

#include <memory>
#include "State.hpp"
#include "Node.hpp"
#include <list>

class Node;
class Geometry;
class Group;
class Transform;
class Camera;
class RenderToTexture;

#ifndef NODE_LIST
#define NODE_LIST
typedef std::list< Node* > NodeList;
#endif //NODE_LIST

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
		virtual void apply(RenderToTexture* tex);
		virtual void apply(Geometry* g);
		virtual void apply(Group* grp);
		virtual void apply(Transform* t);
		virtual void apply(Camera* cam);
		
	protected:
		virtual void init();
		virtual void cleanup();
	private:
		void doTraverse(Node* node);
};


#endif // NODEVISITOR_HPP
