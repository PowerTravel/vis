#ifndef LIGHT_HPP
#define LIGHT_HPP

#include <memory>
#include "Vec4.hpp"
#include "Vec3.hpp"

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
		Light(Vec3 pos, Vec4 ambient, Vec4 diffuse, Vec4 specular, float _attenuation, bool on=true);
		virtual ~Light();

		// Setters and getters for the light attributes
		void setPosition(Vec3 pos);
		Vec3 getPosition();
		void setSpecular(Vec4 spec);
		Vec4 getSpecular();
		void setDiffuse(Vec4 diff);
		Vec4 getDiffuse();
		void setAmbient(Vec4 amb);
		Vec4 getAmbient();
		void setAttenuation(float at);
		float getAttenuation();

		// Turn the light on/off or se if its on
		void on();
		void off();
		bool isOn();


	private:
		bool _on;
		Vec3 _position;
		Vec4 _ambient;
		Vec4 _diffuse;
		Vec4 _specular;
		float _attenuation;
};

#endif // LIGHT_HPP
