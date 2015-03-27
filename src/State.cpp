#include "State.hpp"

const statebit State::ZEROBIT =				0;
const statebit State::_BACK_FACE_CULLING = 	1;
const statebit State::_RENDER_MODE = 		2; // Render point, lines or faces
//const static statebit _COLOR_MODE = 		4;  // Use Textures or Materials
//const static statebit _MATERIAL = 		8;	   

const statebit State::BACK_FACE_CULLING = 	32;
const statebit State::RENDER_MODE_DOT = 	64; 	// = 01
const statebit State::RENDER_MODE_LINE = 	128;  // = 10
const statebit State::RENDER_MODE_FILL = 	192;  // = 11
// const statebit State::COLOR_MODE = 			256;


std::map<statebit, State::attribute> State::create_attrib_map()
{
		
	std::map<statebit, State::attribute> m = 
		  				std::map<statebit, State::attribute>(); 
	m.insert(std::pair<statebit, attribute>(ZEROBIT, State::OFF));
	m.insert(std::pair<statebit, attribute>(RENDER_MODE_DOT, DOT));
	m.insert(std::pair<statebit, attribute>(RENDER_MODE_LINE,LINE));
	m.insert(std::pair<statebit, attribute>(RENDER_MODE_FILL, FILL));
	m.insert(std::pair<statebit, attribute>(BACK_FACE_CULLING, ON));
	return m;
}
const std::map<statebit, State::attribute> State::attribMap = State::create_attrib_map(); 

State::State()
{
	_state = 0;
	_state += BACK_FACE_CULLING; // Turn on backface culling
	_state += RENDER_MODE_FILL;	// Render filled polygons

	_shader = NULL;
}
State::~State()
{

}
void State::merge(State* s)
{
	if(s->isShaderSet())
	{
		setShader(s->getShader());		
	}
	if(s->isBackFaceCullingSet())
	{
		setBackFaceCulling(s->getBackFaceCulling());
	}
	if(s->isRenderModeSet())
	{
		setRenderMode(s->getRenderMode());
	}
}


// Carries the shader.
void State::setShader(shader_ptr s)
{
	_shader =s;
}
shader_ptr State::getShader()
{
	return _shader;
}
bool State::isShaderSet()
{
	return (_shader != NULL);
}
void State::removeShader()
{
	_shader = NULL;
}
	
// Tells if we should cull back faces or not.
void State::setBackFaceCulling(bool c)
{
	if(c){
		setBitToOne(BACK_FACE_CULLING);
	}else{
		setBitToZero(BACK_FACE_CULLING);
	}
	setBitToOne(_BACK_FACE_CULLING);

}
State::attribute State::getBackFaceCulling()
{
	statebit bit = _state & BACK_FACE_CULLING;
	return attribMap.at(bit);
	/*
	if( _state & BACK_FACE_CULLING )
	{
		return ON;
	}else{
		return OFF;	
	}
	*/
}
bool State::isBackFaceCullingSet()
{
	return ( _state & _BACK_FACE_CULLING );
	/*
	if( _state & _BACK_FACE_CULLING )
	{
		return true;
	}else{
		return false;
	}
	*/
}
void State::removeBackFaceCulling()
{	
	setBitToZero(_BACK_FACE_CULLING);
	setBitToOne(BACK_FACE_CULLING);
}

void State::setRenderMode(attribute a)
{
	// Clear the bits
	State::setBitToZero(RENDER_MODE_LINE);
	State::setBitToZero(RENDER_MODE_DOT);
	if(a == DOT)
	{
		setBitToOne(RENDER_MODE_DOT);
	}else if(a == LINE){
		setBitToOne(RENDER_MODE_LINE);		
	}else{
		setBitToOne(RENDER_MODE_FILL);		
	}
}
State::attribute State::getRenderMode()
{
	return attribMap.at(_state & RENDER_MODE_FILL);
}

bool State::isRenderModeSet()
{
	return _state & _RENDER_MODE;
}
void State::removeRenderMode()
{
	setRenderMode(FILL);
	setBitToZero(_RENDER_MODE);
}

void State::setBitToZero(statebit bit)
{
	statebit tmp = _state | bit;
	_state  = tmp - bit;
}

void State::setBitToOne(statebit bit)
{
	_state = _state | bit;
}

