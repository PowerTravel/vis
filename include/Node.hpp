#ifndef NODE_HPP
#define NODE_HPP

#include <memory>
#include "State.hpp"
#include "UpdateCallback.hpp"

class NodeVisitor;

#ifndef NODE_PTR
#define NODE_PTR
class Node;
typedef std::shared_ptr<Node> node_ptr;
#endif // NODE_PTR

/*	
 *	Class: 		Node
 *	Purpose: 	BaseClass for the Scenegraph. 
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
		state_ptr _state;
		callback_ptr _callback;
};


#endif //NODE_HPP
