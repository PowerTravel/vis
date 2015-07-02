#ifndef NODE_HPP
#define NODE_HPP

#include <memory>
#include <list>
#include "State.hpp"
#include "UpdateCallback.hpp"
#include "CollisionGeometry.hpp"

//class NodeApplier;
class PhysicsVisitor;
#ifndef NODE_PTR
#define NODE_PTR
class Node;
typedef std::shared_ptr<Node> node_ptr;
#endif // NODE_PTR

#ifndef CALLBACK_PTR
#define CALLBACK_PTR
typedef std::shared_ptr<UpdateCallback> callback_ptr;
#endif // CALLBACL_PTR

/*	
 *	Class: 		Node
 *	Purpose: 	Abstract BaseClass for the Scenegraph. 
 *	Misc:		All other nodes inherit from this one.
 */
class Node{

	public:

		// Identifyer if the Node is able to have children or not.
		enum N_Type{NODE, GROUP};
		
		Node();
		virtual ~Node();

		// Basic functionality

		virtual void update();
		virtual void acceptVisitor(class NodeVisitor& v) =  0;
		virtual void connectCallback(callback_ptr cb);
		UpdateCallback* getUpdateCallback();

		N_Type getType();
		void setState(State* s);
		State* getState();	

	protected:
		
		N_Type _type;
		State _state;
		callback_ptr _callback;
};


class PhysicsInterface : public Node{
	protected:
		collGeom_ptr _cg_ptr; 
	public:
		PhysicsInterface(){ _cg_ptr = NULL; };
		virtual ~PhysicsInterface(){ _cg_ptr = NULL; };

		virtual void draw() = 0;
		virtual void acceptPhysicsVisitor(PhysicsVisitor& v) = 0;

		collGeom_ptr getCollisionGeometry(){return _cg_ptr;};
		void setCollisionGeometry(collGeom_ptr cg){_cg_ptr = cg;};
};
#endif //NODE_HPP
