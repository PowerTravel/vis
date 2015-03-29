#include "State.hpp"
#include <gl/glew.h>
#include <iostream>

State::State()
{
	_state_map = std::map<Attribute, Value>();
	_shader = NULL;
}
State::~State()
{
	_shader = NULL;
}

void State::merge(State* s)
{	
	if(s ==  NULL)
	{
		return;
	}

	// Add all static attributes
	for(std::map<Attribute, Value>::iterator it = s->_state_map.begin(); it != s->_state_map.end(); it++)
	{
		this->set(it->first ,it->second);
	}

	if(s->contain(Attribute::SHADER))
	{
		//std::cout <<"State::merge(): M=" <<s->_shader->getUniform("M")<< " V= " << s->_shader->getUniform("V") <<" P = "<<s->_shader->getUniform("P") << std::endl;
		this->set(Attribute::SHADER,s->_shader);
	//	_shader = s->_shader; 
	}
}

void State::apply()
{
	// SHADER
	if(_shader != NULL){
		_shader->use();
	}else{
		std::cerr<< "No shader when State::apply() is called, May cause undefined behaviour "<< std::endl;
	}

	// BACK_FACE_CULLING
	glCullFace(GL_BACK);
	if(_state_map.count(Attribute::BACK_FACE_CULLING)>0){
		Value val = _state_map.at(Attribute::BACK_FACE_CULLING);
		if(val == Value::OFF){
			glDisable(GL_CULL_FACE);
		}else if(val == Value::ON){
			glEnable(GL_CULL_FACE);
		}
	// Default
	}else{
		glEnable(GL_CULL_FACE);
	}

	// RENDER_MODE
	if(_state_map.count(Attribute::RENDER_MODE)>0){
		Value val = _state_map.at(Attribute::RENDER_MODE);
		if(val == Value::FILL){
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}else if(val == Value::LINE){
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		
		}else if(val == Value::POINT){
			glPolygonMode(GL_FRONT_AND_BACK,GL_POINT);
		
		}
	// Default
	}else{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	
	// COLOR MODE
	// IMPLEMENT WHEN WE HAVE TEXTURES AND MATERIALS
	if(_state_map.count(Attribute::COLOR_MODE)>0){
		Value val = _state_map.at(Attribute::COLOR_MODE);
		if(val == Value::TEXTURE){
			// RENDER WITH DIFFUSE TEXTURE
		}else if(val == Value::MATERIAL){
			// RENDER WITH MATERIAL
		}
	}else{
		// RENDER WITH DIFFUSE TEXTURE IF IT EXISTS
		// ELSE RENDER WITH MATERIAL IF IT EXISTS
		// ELSE RENDER WITH DEFAULT MATERIAL
	}
}

void State::set(Attribute atr, Value val )
{
	_state_map[atr] = val;
}

void State::set(Attribute atr, shader_ptr p)
{
	_state_map[Attribute::SHADER] = Value::ON;
	_shader = p;
}

bool State::get(Attribute atr, Value& val)
{
	std::map<Attribute, Value>::iterator it = _state_map.find(atr);
	if(it != _state_map.end()){
		val = it->second;
		return true;
	}
	return false;
}

shader_ptr State::getShader(){
	return _shader;
}

bool State::get(Attribute atr, shader_ptr& s)
{
	if(_shader != NULL){
		s = _shader;
		return true;
	}
	return false;
}

bool State::contain(Attribute atr)
{
	return _state_map.count(atr);
}

void State::remove(Attribute atr)
{
	std::map<Attribute, Value>::iterator it = _state_map.find(atr);
	if(it != _state_map.end())
	{
		_state_map.erase(it);
		
		if(atr == Attribute::SHADER){
			_shader = NULL;
		}
	}
}

