#include <iostream>

#include "GlobalUpdateVisitor.hpp"
#include "Transform.hpp"
#include "Camera.hpp"
#include "Geometry.hpp"
#include "ParticleSystem.hpp"

GlobalUpdateVisitor::GlobalUpdateVisitor()
{

}

GlobalUpdateVisitor::~GlobalUpdateVisitor()
{

}
/*
void GlobalUpdateVisitor::apply(Geometry* n)
{
	std::cout << "Geometry" << std::endl;
}

void GlobalUpdateVisitor::apply(ParticleSystem* n)
{
	std::cout << "ParticleSystem" << std::endl;
}

void GlobalUpdateVisitor::apply(Group* n)
{
	std::cout << "Group" << std::endl;
}

void GlobalUpdateVisitor::apply(Transform* n)
{
	std::cout << "Transform" << std::endl;
}

void GlobalUpdateVisitor::apply(Camera* n)
{
	std::cout << "Camera" << std::endl;
}
*/		

void GlobalUpdateVisitor::init(Group* grp)
{
	NodeVisitor::init(grp);
	_rList = std::list<RenderNode>();
	if(  _rList.empty() )
	{	
		_rList.push_back(RenderNode());
		_rit = _rList.begin();
	}
}

void GlobalUpdateVisitor::reset(Group* grp)
{
	NodeVisitor::reset(grp);
	while(!_rList.empty())
	{
		draw(_rList.front());	
		_rList.pop_front();
	}
}


void GlobalUpdateVisitor::apply(ParticleSystem* n)
{

	_rit->setGeometry(n);
	modify_rList(0, NULL,NULL,NULL, n->getState());

	if(send_data_to_shader(n))
	{
//		n->draw();
	}
	decrease_aList();
}

void GlobalUpdateVisitor::apply(Geometry* n)
{
/*
	RenderNode rn = RenderNode();

	rn.setM(aList.front().m);

	// We sync the geometries state with the lates entry in the aList without adding it to the aList.
	state_ptr state = syncStates(aList.front().s.get(),n->getState() );

	

	std::cerr<<"GlobalUpdateVisitor visiting Geometry."<<std::endl;

	//if( state->contain(ST_ATR_SHADER) )
	if( !state->contain(State::Attribute::SHADER) )
	{
		std::cerr<<"No shader present. Not rendering geometry."<<std::endl;
		return;
	}
*/
	_rit->setGeometry(n);
	modify_rList(0, NULL, NULL, NULL, n->getState());


	if(send_data_to_shader(n))
	{
	//	n->draw();
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
void GlobalUpdateVisitor::apply(Group* n)
{
	// This node will never have transformation matrices,
	// But it may have children and a state, thus we add a unit matrix
	// together with the number of children it has.
	modify_aList(n->getNrChildren(), mat4(1.0f) , n->getState());
	
	modify_rList(n->getNrChildren(), NULL, NULL, NULL, n->getState());
}

/*
 * Name:	apply (Camera)
 * Purpose: Adds entries to the aggregate lists
 * Input:	Camera* cam: The camera to be visited
 * Output:	-	
 * Misc:	Assumes we only have one camera. The latest camera
 *			is the only one that will be rendered.
 */
void GlobalUpdateVisitor::apply(Camera* n)
{

	mat4 v = n->getViewMat();
	mat4 p = n->getProjectionMat();
	modify_rList(n->getNrChildren(), NULL, &v, &p, n->getState());

	// We save the View and Projection Matrices
	_V = n->getViewMat();
	_P = n->getProjectionMat();
	// See apply (Group)
	_isModelviewSet = true;
	modify_aList(n->getNrChildren(), mat4(1.0f), n->getState());
}


/*
 * Name:	apply (Transform)	
 * Purpose:	adds its transformationMatrix to the mList as well as
 *			its state.
 * Input:	Transform* t: The transformation node to be visited.	
 * Output:	-	
 * Misc:	-
 */
void GlobalUpdateVisitor::apply(Transform* n)
{	
	mat4 m = n->get();
	modify_rList(n->getNrChildren(), &m, NULL, NULL, n->getState());

	modify_aList(n->getNrChildren(), n->get(), n->getState());
}

void GlobalUpdateVisitor::modify_rList(int count, mat4* m, mat4* v, mat4* p, State* s)
{
	// Update the current RenderNode
	if(m != NULL)
	{
		_rit->setM( *m );
	}
	if(v != NULL)
	{
		_rit->setV(*v);	
	}
	if(p != NULL)
	{
		_rit->setP(*p);	
	}
	if(s != NULL)
	{
		_rit->setState(s);
	}
	
	// If the node has more than one child we make count-1 copies of the current RenderNode
	if(count > 1)
	{
		std::list<RenderNode>::iterator it = _rit;
		it++;
		_rList.insert(it, count-1, *_rit );
	}

	// If we are adding a leaf (aka count == 0), Advance the iterator
	if( (count == 0) && (_rit != _rList.end()) )
	{
		_rit++;
	}
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
void GlobalUpdateVisitor::modify_aList(int count, mat4 m, State* s)
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

void GlobalUpdateVisitor::decrease_aList()
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

state_ptr GlobalUpdateVisitor::syncStates(State* lastState ,State* newState)
{
	state_ptr result = state_ptr(new State());

	result->merge(lastState);
	result->merge(newState);
	
	return result;
}


bool GlobalUpdateVisitor::send_data_to_shader(Node* n)
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

void GlobalUpdateVisitor::draw(VirtualRenderNode& n)
{
	std::cout << "GlobalUpdateVisitor  Drawing" << std::endl;
	n.draw();
}
