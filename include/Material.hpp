#ifndef MATERIAL_HPP
#define MATERIAL_HPP
#include <memory>
#include "MatLib.hpp"
#include <assimp/scene.h>

class Light;
class State;
#ifndef MATERIAL_PTR
#define MATERIAL_PTR
class Material;
typedef std::shared_ptr<Material> material_ptr;
#endif // MATERIAL_PTR

/*
 * Class: 	Material
 * Purpose: Carries information about material and methods to 
 * 			apply them.
 * Misc:	-
 */
class Material{

	public:
		// A few presets
		enum MaterialPreset{
			RANDOM,
			RUBBER_RED,
			PLASTIC_GREEN,
			GOLD,
			OBSIDIAN,
			PEARL
		};

		Material();
		Material(MaterialPreset);
		Material(const aiMaterial* mat);
		Material(float shininess, vec4 specular, vec4 diffuse, vec4 ambient);
		virtual ~Material();

		// Can set materials from presets or from aiMaterial
		void setMaterial(MaterialPreset);
		void setMaterial(const aiMaterial* mat);

		// Getters and setters for not only current values
		// but also when being illuminated
		void setShininess(float s);
		float getShininess();
		void setSpecular(vec4 spec);
		vec4 getSpecular();
		vec4 getSpecular(Light* l);
		void setDiffuse(vec4 diff);
		vec4 getDiffuse();
		vec4 getDiffuse(Light* l);
		void setAmbient(vec4 amb);
		vec4 getAmbient();
		vec4 getAmbient(Light* l);

		void apply(State* st);

	private:

		float _shininess;
		vec4 _specular;
		vec4 _diffuse;
		vec4 _ambient;
};

#endif// MATERIAL_HPP
