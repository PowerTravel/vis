#include "State.hpp"

const statebit State::ZEROBIT 				=   0;
const statebit State::_BACK_FACE_CULLING 	=   1;
const statebit State::_RENDER_MODE 			=   2;
const statebit State::_COLOR_MODE 			=   4;
const statebit State::_MATERIAL 			=   8;	   

const statebit State::BACK_FACE_CULLING_BIT 	=  32;
const statebit State::RENDER_MODE_DOT_BIT 		=  64;
const statebit State::RENDER_MODE_LINE_BIT 		= 128;
const statebit State::RENDER_MODE_FILL_BIT 		= 192;
const statebit State::COLOR_MODE_BIT 			= 256;
const statebit State::MATERIAL_BIT	 			= 512;



std::map<statebit, State::attribute> State::create_attrib_map()
{
		
	std::map<statebit, State::attribute> m = 
		  				std::map<statebit, State::attribute>(); 
	m.insert(std::pair<statebit, attribute>(ZEROBIT, OFF));
	m.insert(std::pair<statebit, attribute>(BACK_FACE_CULLING_BIT, ON));
	m.insert(std::pair<statebit, attribute>(RENDER_MODE_DOT_BIT, DOT));
	m.insert(std::pair<statebit, attribute>(RENDER_MODE_LINE_BIT,LINE));
	m.insert(std::pair<statebit, attribute>(RENDER_MODE_FILL_BIT, FILL));
	m.insert(std::pair<statebit, attribute>(COLOR_MODE_BIT, TEXTURE));
	m.insert(std::pair<statebit, attribute>(MATERIAL_BIT, ON));
	return m;
}
const std::map<statebit, State::attribute> State::attribMap = State::create_attrib_map(); 


State::State()
{
	_state = 0;
	_state += BACK_FACE_CULLING_BIT; // Turn on backface culling
	_state += RENDER_MODE_FILL_BIT;	// Render filled polygons

	_shader = NULL;
}
State::~State()
{

}
void State::merge(State* s)
{
 	
	if(s->_shader != NULL)
	{
		setShader(s->getShader());		
	}
	if(s->_state & _BACK_FACE_CULLING)
	{
		setBackFaceCulling(s->getBackFaceCulling());
	}
	if(s->_state & _RENDER_MODE)
	{
		setRenderMode(s->getRenderMode());
	}
}


void State::apply()
{

}

void set(attribute atr, attribute val )
{
	switch(atr)
	{
		case BACK_FACE_CULLING:
			switch(val){
				case ON:
					setBitToOne(BACK_FACE_CULLING_BIT);
					break;
				case OFF;
					setBitToZero(BACK_FACE_CULLING_BIT);
					break;
				default:
					std::cerr << "Faulty State Set" << std::endl;
					break;
			};
			break;
		
		case RENDER_MODE:
			// We need to set both bits to zero so that if
			// one first sets DOT then later to LINE it does
			// not add upp to FILL
			setBitToZero(RENDER_MODE_FILL_BIT);
			switch(val)
			{
				case DOT:
					setBitToOne(RENDER_MODE_DOT_BIT);
					break;
				case LINE:
					setBitToOne(RENDER_MODE_LINE_BIT);
					break;
				case FILL:
					setBitToOne(RENDER_MODE_FILL_BIT);
					break;
				default;
					std::cerr << "Faulty State Set" << std::endl;
					break;
			};
			break;

		case COLOR_MODE:
			std::cerr << "COLOR_MODE not IMPLEMENTED" << std::endl;
			break;

		default:
			break;
	};
}
void get(shader_ptr p)
{
	p = _shader;
}

void State::setShader(shader_ptr s)
{
	_shader =s;
}

shader_ptr State::getShader()
{
	return _shader;
}

// Tells if we should cull back faces or not.
void State::setBackFaceCulling(bool c)
{
	if(c){
		setBitToOne(BACK_FACE_CULLING_BIT);
	}else{
		setBitToZero(BACK_FACE_CULLING_BIT);
	}
	setBitToOne(_BACK_FACE_CULLING);

}
State::attribute State::getBackFaceCulling()
{
	statebit bit = _state & BACK_FACE_CULLING_BIT;
	return attribMap.at(bit);
}

void State::setRenderMode(attribute a)
{
	// Clear the bits
	State::setBitToZero(RENDER_MODE_LINE_BIT);
	State::setBitToZero(RENDER_MODE_DOT_BIT);
	switch(arrtibute)
	{
		case DOT:
			setBitToOne(RENDER_MODE_DOT_BIT);
			break;
		case LINE:
			setBitToOne(RENDER_MODE_LINE_BIT);		
			break;
		case FILL:
			setBitToOne(RENDER_MODE_FILL_BIT);		
			break;
		default:
			break;
	
	};
}
State::attribute State::getRenderMode()
{
	return attribMap.at(_state & RENDER_MODE_FILL_BIT);
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

statebit State::get_state_bit()
{
	return _state;
}

