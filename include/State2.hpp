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


		// Bitflag definitions
		// '_' before the variable name tells us if that state is set or not
		// variable name without '_' is the actual value of that state attribute
		// State attribute with more that two values takes up more bits
		// For examle rener_mode has 3 possible exclusive values and thus
		// takes up two bits
		
		const static statebit ZEROBIT; //		  =   0 = 	00000000000
		const static statebit _BACK_FACE_CULLING;//=  1 = 	00000000001
		const static statebit _RENDER_MODE;//     =   2 = 	00000000010
		const static statebit _COLOR_MODE; //     =   4 =	00000000100
		const static statebit _MATERIAL; // 	  =   8 =	00000001000

	public:
		const static statebit BACK_FACE_CULLING_BIT;//=  32 =	00000010000
		const static statebit RENDER_MODE_DOT_BIT;//  =  64 = 	00000100000
		const static statebit RENDER_MODE_LINE_BIT;// = 128 = 	00001000000
		const static statebit RENDER_MODE_FILL_BIT;// = 192 = 	00001100000
		const static statebit COLOR_MODE_BIT; // 	  = 256 = 	00010000000
		const static statebit MATERIAL_BIT; //		  = 512 = 	00100000000

		enum attribute{
			// VALUES
			ON,			// Generic ON indicator
			OFF,		// Generic OFF indicator
			DOT,		// Specific to render mode
			LINE,		// --||--
			FILL, 		// --||--

			
			// TYPES	
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

		void set(attribute type, statebit value);
		void set(shader_ptr s);
		//void set(Material m);

		void get(attribute type, attribute* value);
		void get(shader_ptr s);
		//void get(Material* m);
	

		// Tells if we should cull back faces or not.
		void setBackFaceCulling(bool c);
		attribute getBackFaceCulling();
	//	bool isBackFaceCullingSet();
		//void removeBackFaceCulling();
		
		// Tells us how to render polygons
		void setRenderMode(attribute a);
		attribute getRenderMode();
	//	bool isRenderModeSet();
		//void removeRenderMode();

		
		// Carries the shader.
		void setShader(shader_ptr s);
		shader_ptr getShader();
	//	bool isShaderSet();
	//	void removeShader();

	private:

		statebit _state;
		shader_ptr _shader;

//		std::map<attribute, attribute> _state;
	
		const static std::map<statebit, attribute> attribMap;
		static std::map<statebit, attribute> create_attrib_map();	

		void setBitToZero(statebit bit);
		void setBitToOne(statebit bit);
		statebit get_state_bit();

};

#endif // STATE_HPP
