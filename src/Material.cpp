#include <cstdio>
#include "Material.hpp"
#include "State.hpp"
//#include "Light.hpp"
Material::Material()
{
	setMaterial(PEARL);
}

Material::Material(const aiMaterial* mat)
{
	setMaterial(mat);
}
Material::Material(MaterialPreset m)
{
	setMaterial(m);
}

void Material::setMaterial(MaterialPreset m)
{
	if(m == RUBBER_RED)
	{
		_shininess=10;
		_ambient = vec4(0.05, 0,0,1);
		_diffuse= vec4(0.7,0.04,0.04,1);
		_specular = vec4(0.5,0.4,0.4,1);
	}else if(m == GOLD){
		_shininess=51.2;
		_ambient = vec4(0.24725, 0.1995,0.0745,1);
		_diffuse = vec4(0.75164,0.60648,0.22648,1);
		_specular= vec4(0.628281,0.55802,0.366065,1);
	}else if(m == OBSIDIAN){
		_shininess=38.4;
		_ambient = vec4(0.05375, 0.05,0.06625,1);
		_diffuse = vec4(0.18275,0.17,0.22525,1);
		_specular= vec4(0.332741,0.328634,0.3463425,1);
	}else if(m == PLASTIC_GREEN){
		_shininess=32;
		_ambient = vec4(0, 0,0,1);
		_diffuse = vec4(0.1,0.35,0.1,1);
		_specular= vec4(0.5,0.5,0.5,1);
	}else if(m == PEARL){
		_shininess=11;
		_ambient = vec4(0.01, 0.01,0.01,1);
		_diffuse = vec4(1,0.829,0.829,1);
		_specular= vec4(0.3,0.3,0.3,1);
	}
}

Material::Material(float shininess, vec4 specular, vec4 diffuse, vec4 ambient)
{
	_shininess = shininess;
	_specular = specular;
	_diffuse = diffuse;
	_ambient = ambient;
}

Material::~Material()
{

}

/*
 * Name:	setMaterial
 * Purpose: sets the material from an aiMaterial struct
 * Input:	aiMaterial* mat: A pointer to the aiMaterial one gets
 *			from assimp.
 * Output:	-
 * Misc:	-
 */
void Material::setMaterial(const aiMaterial* mat)
{
	aiColor4D spec(0.f,0.f,0.f,0.f);
	aiColor4D diff(0.f,0.f,0.f,0.f);
	aiColor4D amb(0.f,0.f,0.f,0.f);
	if(	AI_SUCCESS == mat->Get(AI_MATKEY_COLOR_SPECULAR, spec) 	&&
		AI_SUCCESS == mat->Get(AI_MATKEY_COLOR_AMBIENT, amb) 	&&
		AI_SUCCESS == mat->Get(AI_MATKEY_COLOR_DIFFUSE, diff)   &&
		AI_SUCCESS == mat->Get(AI_MATKEY_SHININESS, _shininess) ){
	
		_specular = vec4(spec.r, spec.g, spec.b, spec.a);
		_ambient= vec4(amb.r, amb.g, amb.b, amb.a);
		_diffuse = vec4(diff.r, diff.g, diff.b, diff.a);
	
	}else{	
		fprintf(stderr, "Failed to load a material\n");
		setMaterial(RUBBER_RED);
	}
}


// Setters and getters for the Material
void Material::setShininess(float s)
{
	_shininess = s;
}
float Material::getShininess()
{
	return _shininess;
}
void Material::setSpecular(vec4 spec)
{
	_specular = spec;
}
vec4 Material::getSpecular()
{
	return _specular;
}

// Calculates the specular product when shone on by a light.
vec4 Material::getSpecular(Light* l)
{
	vec4 lSpec = l->getSpecular();
	return vec4( 	lSpec[0]*_specular[0],
					lSpec[1]*_specular[1],
					lSpec[2]*_specular[2],
					lSpec[3]*_specular[3]);
}
void Material::setDiffuse(vec4 diff)
{
	_diffuse = diff;
}
vec4 Material::getDiffuse()
{
	return _diffuse;
}
// Calculates the diffuse product when shone on by a light.
vec4 Material::getDiffuse(Light* l)
{
	vec4 lDiff = l->getDiffuse();
	return vec4( 	lDiff[0]*_diffuse[0],
					lDiff[1]*_diffuse[1],
					lDiff[2]*_diffuse[2],
					lDiff[3]*_diffuse[3]);
}
void Material::setAmbient(vec4 amb)
{
	_ambient = amb;
}
vec4 Material::getAmbient()
{
	return _ambient;
}
// Calculates the ambient product when shone on by a light.
vec4 Material::getAmbient(Light* l)
{
	vec4 lAmb = l->getAmbient();
	return vec4( 	lAmb[0]*_ambient[0],
					lAmb[1]*_ambient[1],
					lAmb[2]*_ambient[2],
					lAmb[3]*_ambient[3]);
}

// Given a state it applies itself to it
void Material::apply(State* st)
{
	Material* mat = new Material();
	*mat = *this;
	material_ptr m = material_ptr(mat);
	st->set(State::Attribute::MATERIAL, m);
}

