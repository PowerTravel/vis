#include "Node.hpp"
#include "Group.hpp"
#include <iostream> // Debug

Node::Node()
{
	_state = State();
	_type = NODE;
	_callback = NULL;	

	_parentList = std::list<Group*>();
	_pit = _parentList.begin();
}

Node::~Node()
{
	_callback = NULL;
}

/*
 * Name:	Update
 * Purpose: Updates the aspects of the node if it has a callback
 * Input:	-
 * Output:	-
 * Misc:	-
 */
void Node::update()
{
	if( _callback!= NULL){
		_callback->execute();
	}
}

/*
 * Name:	connectCallback
 * Purpose:	Connects a callback to the node.
 * Input:	std::shared_ptr<UpdateCallback>: The callback base class.
 * Output:	-
 * Misc:	-
 */
void Node::connectCallback(callback_ptr cb)
{
	_callback = cb;
}

/*
 * Name:	getUpdateCallback
 * Purpose:	returns a pointer to the attached callback
 * Input:	-
 * Output:	UpdateCallbacl*: A pointer to the attached Callback
 * Misc:	Return NULL if there is no callback attached.
 */
UpdateCallback* Node::getUpdateCallback()
{
	return _callback.get();
}

/*
 * Name:	setState
 * Purpose:	Updates the state of the Node
 * Input:	State* s: A pointer to the state
 * Output:	-
 * Misc:	The state is not copied over to the NODE.
 *			it is merged with the existing state of the NODE.
 *			The node will retain all of its previous set state-
 *			-attributes and take on the new ones of the new state.
 *			Can be called multiple times to accumulate state 
 *			attributes.
 */
void Node::setState(State* s)
{
	if(s != NULL){
		_state.merge(s);
		dirty(STATE);
	}
}

/*
 * Name: 	getState
 * Purpose:	Returns a pointer to the state of the NODE for direct
 *			manipulation.
 * Input:	-
 * Output:	State*: A pointer to the nodes state.
 * Misc:	-
 */
State* Node::getState()
{
	return &_state;
}

/*
 * Name:	getType
 * Purpose:	gets the Type of the node, which tells us if it is able
 *			to have children or not.
 * Input: 	-
 * Output:	Node::N_Type: the type of the node.
 * Misc:	Type can be 'NODE' or 'GROUP' The difference between them
 * 			being that GROUP can have children while NODE cannot.
 *			It is set at creation and cannot be changed later.
 */
Node::N_Type Node::getType()
{
	return _type;
}

void Node::reset()
{
	_pit = _parentList.begin();	
}
		
int Node::getNrParents()
{
	return _parentList.size();
}

void Node::addParent(Group* grp)
{
	_parentList.push_back(grp);
	if(_parentList.size() == 1)
	{
		_pit = _parentList.begin();
	}
	dirty(REBUILD);
}

Group* Node::getParent()
{
	if(_parentList.empty() || (_pit == _parentList.end()) )
	{
		return NULL;
	}
	return *_pit;	
}

void Node::firstParent()
{
	_pit = _parentList.begin();
}

void Node::nextParent()
{
	auto it = _pit;
	it ++;
	if(it != _parentList.end() )
	{
		_pit ++;	
	}
}

void Node::dirty(dirty_bit bit)
{
	// Cant accept CLEAN bit
	if(bit == CLEAN)
	{
		return;
	}
	// We set the bit if it is not already set
	if( (bit & _dFlag) != bit )
	{
		
		// Bits that exist in both bit and _dFlag
		int same = _dFlag & bit; 
		// Bits that exists in either but not both
		int different = _dFlag ^ bit; 	
		_dFlag = same +  different;
//	std::cout << "Node::Dirty: " << bit << std::endl;

		for(auto it = _parentList.begin(); it != _parentList.end(); it++)
		{
			(*it)->dirty(PATH);
		}
	}else{
		// If it IS already set we return
		return;
	}
}

void Node::clean(dirty_bit bit)
{
	if(bit==CLEAN)
	{
		_dFlag = CLEAN;
	}else{
		if(_dFlag & bit)
		{
			// Picks filetrs bits thate are in both bit and _dFlag
			int tmpBit = _dFlag & bit;
			// Sets all bits that exists in both _dflag and tmpBit to 0
			_dFlag = _dFlag ^ tmpBit;
		}
	}
}

int Node::getDirtyFlag()
{
	return _dFlag;
}

void Node::printFlag()
{
	std::cout << "Node::flag: ";
	if( (_dFlag & STATE)  )
	{
		std::cout << "STATE, ";
	}		
	
	if( (_dFlag & TRANSFORM)  )
	{
		std::cout << "TRANSFORM, ";
	}		
	
	if( (_dFlag & CAM)  )
	{
		std::cout << "CAM, ";
	}		
	
	if( (_dFlag & REBUILD)  )
	{
		std::cout << "REBUILD, ";
	}	
	
	if( (_dFlag & PATH)  )
	{
		std::cout << "PATH ";
	}	
	std::cout << std::endl;
}

void Node::printFlagChain()
{
	printFlag();
	Group* grp = getParent();
	if( grp != NULL )
	{
		grp->printFlagChain();
	}
}
