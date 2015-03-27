#ifndef STATE_HPP
#define STATE_HPP

#include <GL/glew.h>
#include <memory>
#include <list>
#include <map>
#include "Shader.hpp"
#include "Material.hpp"
#include "Light.hpp"
#include "Texture.hpp"

#ifndef STATE_PTR
#define STATE_PTR
class State;
typedef std::shared_ptr<State> state_ptr;
#endif // STATE_PTR

/*
 * Class:	State
 * Purpose: Carries all attributes common to all Nodes
 * Misc:	You don't set states in the usual sense.
 *			Instead you merge them together. If two states
 *			are set they are overridden.
 */
class State{

	public:
		
		State();
		virtual ~State();

		enum PolygonMode{
			POINT,
			LINE,
			FILL
		};

		enum ShadeType{
			PHONG,
		}; 

		enum TexMap{
			DIFFUSE,
			SPECULAR,
			SHADOW
		};

		// Setters and getters for unique attributes

		// Tells if the geometry be rendered as lines, dots or 
		// triangles.
		void setPolygonMode(PolygonMode m);
		PolygonMode getPolygonMode();
		bool isPolygonModeSet();
		void removePolygonMode();

		// Tells if we should render with flat shading, Phong
		// or Blinn Phong. Currently only Phong shading is supported.
		void setShadeType(ShadeType type);
		ShadeType getShadeType();
		bool isShadeTypeSet();
		void removeShadeType();

		// Tells if we should cull back faces or not.
		void setCullFace(bool c);
		bool getCullFace();
		bool isCullFaceSet();
		void removeCullFace();
	
		// This tells us if we should render materials or textures.
		void setColorMaterial(bool c);
		bool getColorMaterial(); 
		bool isColorMaterialSet();
		void removeColorMaterial();

		// Carries a material.
		void setMaterial(Material m);
		Material getMaterial();
		bool isMaterialSet();
		void removeMaterial();
		bool isMaterialOn();
		void toggleMaterial();

		// Carries the shader.
		void setShader(shader_ptr s);
		shader_ptr getShader();
		bool isShaderSet();
		void removeShader();


		// Accumulative elements

		// 3 lights are supported at the moment.
		// To add support for more one has to increase
		// the size of the light-uniforms in the fragment
		// and vertex shader
		void pushLight(Light l);
		int getNrLights();
		Light getLight(int n);
		void popLight(int n);

		// One diffuse texture is supported at the moment.
		bool hasTexture(TexMap m);
		void addTexture(TexMap m,Texture t);
		int getNrTextures();
		Texture getTexture(TexMap m);
		void popTexture(TexMap m);

		// Sets the GL_state to repressent this state.
		void apply();

		// Merges two states. If two of the same unique attributes 
		// are set s will override this.
		// If they both carry cumulative attributes they will get 
		// added together
		void merge(State* s); 

		std::vector<TexMap> getMapVector();
	private:

		// Unique attributes
		PolygonMode _polyMode;
		bool _isPolyModeSet;

		ShadeType _shadeType;
		bool _isShadeTypeSet;

		bool _cullFace;
		bool _isCullFaceSet;

		bool _colorMaterial;
		bool _isColorMaterialSet;

		Material _material;
		bool _isMaterialSet;

		shader_ptr _shader;
		bool _isShaderSet;

		// Cumulative attributes
		std::list< Light > _lights;

		std::map<int, Texture> _textures;
};

#endif // STATE_HPP
