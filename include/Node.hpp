#ifndef NODE_HPP
#define NODE_HPP

#include <memory>
#include <list>
#include "State.hpp"
#include "UpdateCallback.hpp"
//#include "BoundingBox.hpp"

class Group;
class NodeVisitor;

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
		virtual void reset();

		virtual void update();
		virtual void acceptVisitor(class NodeVisitor& v) =  0;
		virtual void connectCallback(callback_ptr cb);
		UpdateCallback* getUpdateCallback();

		N_Type getType();
		void setState(State* s);
		State* getState();	

		int getNrParents();
		Group* getParent();
		void firstParent();
		void nextParent();

		void addParent(Group* grp);

		// DEBUG
	protected:
		
		N_Type _type;
		int _dFlag; // Dirty Flag
		State _state;
		callback_ptr _callback;
		
		std::list<Group*> _parentList;
		std::list<Group*>::iterator _pit;
		
};

class VirtualRenderNode : public Node{
	public:
		virtual void draw() = 0;
};


#endif //NODE_HPP
