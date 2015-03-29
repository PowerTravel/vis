#ifndef STATE_HPP
#define STATE_HPP

#include <GL/glew.h>
#include <memory>
#include <vector>
#include <map>

#include "Shader.hpp"
//#include "Material.hpp"
//#include "Light.hpp"
//#include "Texture.hpp"

#ifndef _STATE_PTR
#define _STATE_PTR
class State;
typedef std::shared_ptr<State> state_ptr;
#endif // _STATE_PTR

class State{
	
	public:
	
		enum class Attribute{
			// ATTRIBTUE
			SHADER,				// Value: ON
			BACK_FACE_CULLING, 	// Value: ON, OFF
			RENDER_MODE,	   	// Value: POINT, LINE, FILL
			COLOR_MODE		   	// Value: TEXTURE, MATERIAL


			// NOT IMPLEMENTED
			//MATERIAL, 		// Value: ON
			//TEXTURE_DIFFUSE, 	// Value: ON
			//TEXTURE_NORMAL,	// Value: ON
			//TEXTURE_SHADOW 	// Value: ON
		};

		enum class Value{
			// VALUES
			ON,			// Value to BACK_FACE_CULLING, MATERIAL
			OFF,		// Value to BACK_FACE_CULLING, MATERIAL
			
			POINT,		// Value to RENDER_MODE
			LINE,		// Value to RENDER MODE
			FILL,		// Value to RENDER_MODE
			
			TEXTURE,	// Value to COLOR_MODE
			MATERIAL,	// Value to COLOR_MODE
		};

		State();
		virtual ~State();

		void merge(State* s);
		void apply();

		void set(Attribute atr, Value Value);
		void set(Attribute atr, shader_ptr s);

		bool get(Attribute atr, Value& val);
		bool get(Attribute atr, shader_ptr& s);

		bool contain(Attribute atr);

		void remove(Attribute atr);
	
		shader_ptr getShader();


	private:

		shader_ptr _shader;
		std::map<Attribute, Value> _state_map;
};
/*
typedef State::Attribute::SHADER 			ST_ATR_SHADER;
typedef State::Attribute::BACK_FACE_CULLING	ST_ATR_BACK_FACE_CULLING; 
typedef State::Attribute::RENDER_MODE		ST_ATR_RENDER_MODE;
typedef State::Attribute::COLOR_MODE		ST_ATR_COLOR_MODE;
typedef State::Value::ON			ST_VAL_ON;	
typedef State::Value::OFF			ST_VAL_OFF;
typedef State::Value::POINT			ST_VAL_POINT;
typedef State::Value::LINE			ST_VAL_LINE;
typedef State::Value::TEXTURE		ST_VAL_TEXTURE;
typedef State::Value::MATERIAL		ST_VAL_MATERIAL;
*/

#endif // STATE_HPP
