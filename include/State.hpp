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

#ifndef _STATE_BIT
#define _STATE_BIT
typedef unsigned int statebit;
#endif // _STATE_BIT

class State{

	public:

		enum attribute{
			ON,			// Generic ON indicator
			OFF,		// Generic OFF indicator
			DOT,		// Specific to render mode
			LINE,		// --||--
			FILL, 		// --||--
			TEXTURE,	// Specific to color mode
			MATERIAL 	// --||--
		};

		State();
		virtual ~State();

		void merge(State* s);
		
		// Carries the shader.
		void setShader(shader_ptr s);
		shader_ptr getShader();
		bool isShaderSet();
		void removeShader();

		// Tells if we should cull back faces or not.
		void setBackFaceCulling(bool c);
		attribute getBackFaceCulling();
		bool isBackFaceCullingSet();
		void removeBackFaceCulling();
		
		// Tells us how to render polygons
		void setRenderMode(attribute a);
		attribute getRenderMode();
		bool isRenderModeSet();
		void removeRenderMode();


	private:

		statebit _state;
		shader_ptr _shader;
		const static std::map<statebit, attribute> attribMap;
		static std::map<statebit, attribute> create_attrib_map();	

		void setBitToZero(statebit bit);
		void setBitToOne(statebit bit);


		// Bitflag definitions
		// '_' before the variable name tells us if that state is set or not
		// variable name without '_' is the actual value of that state attribute
		// State attribute with more that two values takes up more bits
		// For examle rener_mode has 3 possible exclusive values and thus
		// takes up two bits
		const static statebit ZEROBIT; //= 0;
		const static statebit _BACK_FACE_CULLING;// = 1;
		const static statebit _RENDER_MODE;// = 2; // Render point, lines or faces
	
		const static statebit BACK_FACE_CULLING;// = 32;
		const static statebit RENDER_MODE_DOT;// = 64; 	// = 01
		const static statebit RENDER_MODE_LINE;// = 128;  // = 10
		const static statebit RENDER_MODE_FILL;// = 192;  // = 11
};

#endif // STATE_HPP
