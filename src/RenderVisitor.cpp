#include <GL/glew.h>
#include <glm/ext.hpp>
#include "RenderVisitor.hpp"
#include "Transform.hpp"
#include "Geometry.hpp"
#include "ParticleSystem.hpp"
#include "Camera.hpp"
#include <iostream>


RenderVisitor::RenderVisitor()
{
	std::list<aggregate_data> aList;
	_isModelviewSet = false;
}

RenderVisitor::~RenderVisitor()
{

}

void RenderVisitor::apply(ParticleSystem* n)
{
	if(send_data_to_shader(n))
	{
		n->draw();
	}
	decrease_aList();
}

bool RenderVisitor::send_data_to_shader(Node* n)
{
	_M = aList.front().m;

	// We sync the geometries state with the lates entry in the aList without adding it to the aList.
	state_ptr state = syncStates(aList.front().s.get(),n->getState() );

	//if( state->contain(ST_ATR_SHADER) )
	if( !state->contain(State::Attribute::SHADER) )
	{
		std::cerr<<"No shader present. Not rendering geometry."<<std::endl;
		return false;
	}
	// Apply the state to openGL
	state->apply();

	// Send the Model View and projection matrices to the shader
	shader_ptr shader;
	state->get(State::Attribute::SHADER, shader);
	shader->setUniformMatrix("M", 1, &_M[0][0]);
	shader->setUniformMatrix("V", 1, &_V[0][0]);
	shader->setUniformMatrix("P", 1, &_P[0][0]);

	return true;
}

/*
 * Name:	apply (Geometry)
 * Purpose: Renders a geometry with the accumulated transformation
 *			matrix and the accumulated state.
 * Input:	Geometry* g: The geometry that is to be rendered
 * Output:	-	
 * Misc:	-
 */
void RenderVisitor::apply(Geometry* n)
{
	if(send_data_to_shader(n))
	{
		n->draw();
	}
	decrease_aList();
}

/*
 * Name:	Apply (Group)	
 * Purpose: Add entries to the aggregate lists. 
 * Input:	Group* grp: The group to be visited
 * Output:	-
 * Misc:	-
 */
void RenderVisitor::apply(Group* n)
{
	// This node will never have transformation matrices,
	// But it may have children and a state, thus we add a unit matrix
	// together with the number of children it has.
	modify_aList(n->childList.size(), mat4(1.0f) , n->getState());
}

/*
 * Name:	apply (Camera)
 * Purpose: Adds entries to the aggregate lists
 * Input:	Camera* cam: The camera to be visited
 * Output:	-	
 * Misc:	Assumes we only have one camera. The latest camera
 *			is the only one that will be rendered.
 */
void RenderVisitor::apply(Camera* n)
{
	// We save the View and Projection Matrices
	_V = n->getViewMat();
	_P = n->getProjectionMat();
	_isModelviewSet = true;

	// See apply (Group)
	modify_aList(n->childList.size(), mat4(1.0f), n->getState());
}


/*
 * Name:	apply (Transform)	
 * Purpose:	adds its transformationMatrix to the mList as well as
 *			its state.
 * Input:	Transform* t: The transformation node to be visited.	
 * Output:	-	
 * Misc:	-
 */
void RenderVisitor::apply(Transform* n)
{	
	modify_aList(n->childList.size(), n->get(), n->getState());
}

/*
 * Name:	modify_aList	
 * Purpose: Adds or removes entries in the aList
 * Input:	-int count: The number of children the added Hmat
 *			will be relevant for.
 *			-mat4 m: The matrix to be summed up.
 *			-State* s: The state that should be accumulated
 *			instead of to the screen
 * Output:	-
 * Misc:	-
 */
void RenderVisitor::modify_aList(int count, mat4 m, State* s)
{
	// If the node is a leaf
	if(count == 0)
	{	
		// If we are in a leaf we have to popp all entries
		// that corresponds to already visited nodes with
		// no children. Those entries are the ones where 
		// 'count' has been reduced to 1; 
		decrease_aList();

	// If the node has children we push the matrix
	}else{
		aggregate_data result;
		result.count = count;	
		
		if(aList.empty()){
			// or if this is the first entry we add it as is
			result.m = m;
			
			result.s = state_ptr(new State());
			result.s->merge(s);

		}else{
			// sum the two latest matrices
			result.m = aList.front().m*m ;
			
			state_ptr oldState = aList.front().s;
			result.s = syncStates(oldState.get(), s);
		}	

		aList.push_front(result);
	}
}


/*
 * Name:	decrease_aList	
 * Purpose:	Pops entries that corresponds to already visited nodes.
 * 			Those are the entries that have had their 'count' reduced
 *	 		to 1. Also, reduce the next remaining entrys count 
 *			by 1 indicating that one of its childs subtree has been
 *			processed.
 * Input:	-
 * Output:	-
 * Misc:	-
 */
void RenderVisitor::decrease_aList()
{	
	std::list<aggregate_data>::iterator it=aList.begin();
	// pop all entries with count reduced to 1.
	while( it->count <= 1 && it!= aList.end() )
	{
		it++;
		aList.pop_front();
	}
	// Reduce the count of the remaing top with one
	aList.front().count--;
}

/*
 * Name:	syncStates
 * Purpose: merges two states into a new state without affecting the
 *	 		input states.
 * Input:	State* lastState: The state that gets its attributes
 *			overridden.
 *			State* newState: The state that gets to override lastState
 * Output:	std::shared_ptr<State>: The sum of the previous states.
 * Misc:	-	
 */
state_ptr RenderVisitor::syncStates(State* lastState ,State* newState)
{
	state_ptr result = state_ptr(new State());

	result->merge(lastState);
	result->merge(newState);
	
	return result;
}
