#ifndef RENDER_VISITOR_HPP
#define RENDER_VISITOR_HPP

#include "NodeVisitor.hpp"
#include "MatLib.hpp"
#include <list>


/*	
 *	Class: 		RenderVisitor
 *	Purpose: 	Traverses the scenegraph and renders it.
 *	Misc:		Derived from nodeVisitor. This is quite a 
 *				complex class that does very different things
 *				depending on which node it is visiting
 *				and the different nodes interact with eachother.
 */
class RenderVisitor : public NodeVisitor{

	public:

		RenderVisitor();
		virtual ~RenderVisitor();
		
		void apply(Geometry* n);
		void apply(Group* n);
		void apply(Transform* n);
		void apply(Camera* n);

	
	private:

		// The struct that carries the data gatherd from all
		// the nodes the RenderVisitor visits.
		struct aggregate_data{
			mat4 m;	// The composite model matrix
			state_ptr s;	// The composite state 
			int count;		// The number of unvisited children;
		};

		// A list containing the updated aggregate_data at each node.
		std::list<aggregate_data> aList;

		mat4 _M; // Model matrix
		mat4 _V; // View matrix
		mat4 _P; // Projection matrix
		bool _isModelviewSet;
		
		// Manipulates the aList
		void modify_aList(int count, mat4 m, State* s);
		void decrease_aList();
	
		state_ptr syncStates(State* lastState, State* newState);

};

#endif // RENDER_VISITOR_HPP
