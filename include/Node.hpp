#ifndef NODE_HPP
#define NODE_HPP

#include <memory>
#include <list>
#include "State.hpp"
#include "UpdateCallback.hpp"
#include "BoundingBox.hpp"

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
		
		enum dirty_bit{
			CLEAN = 0,
			STATE = 1,
			TRANSFORM = 2
		};

		
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

		void getBoundingBoxCorners(double* points);

		int getNrParents();
		void addParent(Group* grp);
		Group* getParent();
		void firstParent(Group* grp);
		void nextParent(Group* grp);

	protected:
		struct global_data{
				
			global_data(){
				m=mat4(1.0);
				s=State();
				bb = BoundingBox(); 
				};

			mat4 m;			// The composite model matrix
			State s;		// The composite state 
			BoundingBox bb; // The bounding box
		};


		N_Type _type;
		State _state;
		callback_ptr _callback;
		
		std::list<Group*> _parentList;
		std::list<Group*>::iterator _pit;

		std::list<global_data> _globals;
		std::list<global_data>::iterator _glit;
};


#endif //NODE_HPP
