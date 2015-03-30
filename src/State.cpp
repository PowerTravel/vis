#include "State.hpp"
#include <gl/glew.h>
#include <iostream>

State::State()
{
	_state_map = std::map<Attribute, Value>();
	_shader = NULL;
	_material = NULL;
}
State::~State()
{
	_shader = NULL;
	_material = NULL;
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
		this->set(Attribute::SHADER,s->_shader);
	}
	
	if(s->contain(Attribute::MATERIAL))
	{
		this->set(Attribute::MATERIAL,s->_material);
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

	
	Light l = Light();
	Material m = Material(Material::PEARL);
	if(_material != NULL){
		m = *_material;
	}
	vec4 diff = m.getDiffuse(&l);
	vec4 amb  =	m.getAmbient(&l); 
	vec4 spec = m.getSpecular(&l);
	vec3 lPos = l.getPosition();
	float att = l.getAttenuation();
	float shi = m.getShininess();
	if(_shader != NULL)
	{
		_shader->setUniform4f("Diff", 1, &diff[0]);
		_shader->setUniform4f("Amb", 1, &amb[0]);
		_shader->setUniform4f("Spec", 1, &spec[0]);
		_shader->setUniform3f("lPos", 1, &lPos[0]);
		_shader->setUniform1f("att", 1, &att);
		_shader->setUniform1f("shi", 1, &shi);
	}


	/*
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
	*/
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

void State::set(Attribute atr, material_ptr m)
{
	_state_map[Attribute::MATERIAL] = Value::ON;
	_material = m;
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

bool State::get(Attribute atr, shader_ptr& s)
{
	if(_shader != NULL){
		s = _shader;
		return true;
	}
	return false;
}

bool State::get(Attribute atr, material_ptr& m)
{
	if(_material != NULL){
		m = _material;
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
		
		if(atr == Attribute::MATERIAL){
			_material = NULL;
		}
	}
}

