#ifndef LIGHT_HPP
#define LIGHT_HPP

#include <memory>
#include "MatLib.hpp"

class Material;
#ifndef LIGHT_PTR
#define LIGHT_PTR
class Light;
typedef std::shared_ptr<Light> light_ptr;
#endif // LIGHT_PTR


/*	
 *	Class: 		Light
 *	Purpose: 	Carries information about the light
 *	Misc:
 */
class Light{

	public:
		Light();
		Light(vec3 pos, vec4 ambient, vec4 diffuse, vec4 specular, float _attenuation, bool on=true);
		virtual ~Light();

		// Setters and getters for the light attributes
		void setPosition(vec3 pos);
		vec3 getPosition();
		void setSpecular(vec4 spec);
		vec4 getSpecular();
		void setDiffuse(vec4 diff);
		vec4 getDiffuse();
		void setAmbient(vec4 amb);
		vec4 getAmbient();
		void setAttenuation(float at);
		float getAttenuation();

		// Turn the light on/off or se if its on
		void on();
		void off();
		bool isOn();


	private:
		bool _on;
		vec3 _position;
		vec4 _ambient;
		vec4 _diffuse;
		vec4 _specular;
		float _attenuation;
};

#endif // LIGHT_HPP
