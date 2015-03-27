#ifndef STATE_HPP
#define STATE_HPP

#include <GL/glew.h>
#include <memory>
#include <list>
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
		
		enum attribute{
			// VALUES
			ON,			// Value to BACK_FACE_CULLING, MATERIAL
			OFF,		// Value to BACK_FACE_CULLING, MATERIAL
			DOT,		// Value to RENDER_MODE
			LINE,		// --||--
			FILL, 		// --||--

			
			// ATTRIBTUE
			BACK_FACE_CULLING, // args: ON, OFF
			RENDER_MODE,	   // args: DOT, LINE, FILL
			COLOR_MODE,		   // args: TEXTURE, MATERIAL

			// BOTH
			TEXTURE, // type args: DIFFUSE, SHADOW, NORMAL mm
			MATERIAL // type args: ON, OFF.
		};

		State();
		virtual ~State();

		void merge(State* s);
		void apply();

		void set(attribute atr, attribute value);
		void set(shader_ptr s);
		//void set(Material m);

		void get(attribute type, attribute* value);
		void get(shader_ptr s);
		//void get(Material* m);

		void remove(attribute type);
	
	private:

		shader_ptr _shader;
		std::map<attribute, attribute> _state_map;
};

#endif // STATE_HPP
