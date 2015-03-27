#include "Light.hpp"
#include "Material.hpp"
Light::Light()
{
	_on = true;
	_position = Vec3(4,4,4);
	_ambient = Vec4(0.7,0.7,0.7,1);
	_diffuse = Vec4(0.7,0.7,0.7,1);
	_specular = Vec4(0.7,0.7,0.7,1);
	_attenuation = 0.2;
}

Light::Light(Vec3 position, Vec4 ambient, Vec4 diffuse, Vec4 specular, float attenuation, bool on)
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


void Light::setPosition(Vec3 pos)
{
	_position = pos;
}
Vec3 Light::getPosition()
{
	return _position;
}
void Light::setSpecular(Vec4 spec)
{
	_specular = spec;
}
Vec4 Light::getSpecular()
{
	if(_on){
		return _ambient;
	}else{
		return Vec4();
	}
}
void Light::setDiffuse(Vec4 diff)
{
	_diffuse = diff;
}
Vec4 Light::getDiffuse()
{
	if(_on){
		return _diffuse;
	}else{
		return Vec4();
	}
}
void Light::setAmbient(Vec4 amb)
{
	_ambient = amb;
}
Vec4 Light::getAmbient()
{
	if(_on){
		return _ambient;
	}else{
		return Vec4();
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
