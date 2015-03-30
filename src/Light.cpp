#include "Light.hpp"
#include "Material.hpp"
Light::Light()
{
	_on = true;
	_position = vec3(4,4,4);	
	_ambient = vec4(0.7,0.7,0.7,1);
	_diffuse = vec4(0.7,0.7,0.7,1);
	_specular = vec4(0.7,0.7,0.7,1);
	_attenuation = 0.2;
}

Light::Light(vec3 position, vec4 ambient, vec4 diffuse, vec4 specular, float attenuation, bool on)
{
	_position = position;
	_ambient = ambient;
	_diffuse = diffuse;
	_specular = specular;
	_attenuation = attenuation;
	_on = on;
}

Light::~Light()
{

}


void Light::setPosition(vec3 pos)
{
	_position = pos;
}
vec3 Light::getPosition()
{
	return _position;
}
void Light::setSpecular(vec4 spec)
{
	_specular = spec;
}
vec4 Light::getSpecular()
{
	if(_on){
		return _ambient;
	}else{
		return vec4();
	}
}
void Light::setDiffuse(vec4 diff)
{
	_diffuse = diff;
}
vec4 Light::getDiffuse()
{
	if(_on){
		return _diffuse;
	}else{
		return vec4();
	}
}
void Light::setAmbient(vec4 amb)
{
	_ambient = amb;
}
vec4 Light::getAmbient()
{
	if(_on){
		return _ambient;
	}else{
		return vec4();
	}
}
void Light::on()
{
	_on = true;
}
void Light::off()
{
	_on = false;
}
bool Light::isOn()
{
	return _on;
}
void Light::setAttenuation(float at)
{
	_attenuation = at;
}
float Light::getAttenuation()
{
	return _attenuation;
}

