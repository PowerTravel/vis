#ifndef GROUP_HPP
#define GROUP_HPP

#include "Node.hpp"

class NodeVisitor;

#ifndef NODE_LIST
#define NODE_LIST
typedef std::list< std::shared_ptr<Node> > NodeList;
#endif // NODE_LIST


#ifndef GROUP_PTR
#define GROUP_PTR
class Group;
typedef std::shared_ptr<Group> group_ptr;
#endif // GROUP_PTR


/*	
 *	Class: 		Group
 *	Purpose: 	A node that can have children.
 *	Misc:		All other child-bearing nodes derive from this one.
 */
class Group : public Node{

	public:

		Group();
		virtual	~Group();

		void destroy();
		void reset();

		void acceptVisitor(NodeVisitor& v);
		
		void updateBoundingBox();


		// Navigation
		void addChild( std::shared_ptr<Node> nodePtr);
		int getNrChildren();
		Node* getChild();
		void nextChild();
		void firstChild();
		
	protected:
		NodeList _childList;
		NodeList::iterator _cit;
		// Data that depends on the rest of the scenegraph

	private:

};
#endif // GROUP_HPP
