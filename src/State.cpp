#include "State.hpp"


State::State()
{
	_state_map = std::map<attribute, attribute>(); 
	_shader = NULL;
}
State::~State()
{

}
void State::merge(State* s)
{
	// Get a list of keys that exist in s and add them to this
}

void State::apply()
{
	// This will be a big if chain.	
}

void State::set(attribute atr, attribute val )
{
	_state_map.insert(std::pair<attribute,attribute>(atr,val));
}

void State::set(shader_ptr p)
{
	_shader = p;
}

void State::get(shader_ptr p)
{
	p = _shader;
}

void State::remove(attribute atr)
{
	// remove atr from state_map
}
