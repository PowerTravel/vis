#include "State.hpp"
State::State()
{
	_isPolyModeSet = false;
	_isShadeTypeSet = false;
	_isCullFaceSet = false;
	_isColorMaterialSet = false;
	_isMaterialSet = false;
	_isShaderSet = false;
	_polyMode = FILL;
	_shadeType = PHONG;
	_cullFace = true;
	_colorMaterial = false;
	_material = Material();
	_isMaterialSet = false;
	_shader = NULL;

	_lights = std::list< Light >();
	_textures = std::map<int, Texture>();
}

State::~State()
{

}


/*
 * Name:	merge
 * Purpose: Merges this state with another. If identical unique 
 * 			attributes are set by both the attribute in this state
 *			is overridden by the one in 's'. Cumulative attributes
 *			such as textures and lights are added.
 * Input:	State* s: The state to be merged with this one.	
 * Output:	-
 * Misc:	-
 */
void State::merge(State* s)
{
	if(s->isPolygonModeSet())
	{
		setPolygonMode( s->getPolygonMode() );
	}
	if( s->isShadeTypeSet() )
	{
		setShadeType( s->getShadeType() );
	}
	if( s->isCullFaceSet() )
	{
		setCullFace( s->getCullFace() );
	}
	if( s->isColorMaterialSet() )
	{
		setColorMaterial(s->getColorMaterial());
	}
	if( s->isMaterialSet() )
	{
		setMaterial( s->getMaterial() );
	}
	if(s->isShaderSet())
	{
		setShader(s->getShader());
	}

	int n = s->getNrLights();
	for(int i=0; i<n; i++)
	{
		pushLight(s->getLight(i));
	}


	std::vector<TexMap> mv = getMapVector();
	n = mv.size();
	for(int i =0; i<n; i++ )
	{
		if(s->hasTexture(mv[i]))
		{
			addTexture(mv[i], s->getTexture(mv[i]) );
		}	
	}
	
}

std::vector<State::TexMap> State::getMapVector()
{
	std::vector<TexMap> mv = std::vector<TexMap>();
	mv.push_back(State::DIFFUSE);
	mv.push_back(State::SPECULAR);
	mv.push_back(State::SHADOW);
	return mv;
}

// Below are setters, getter and inspectors for the unique
// attributes. Se the hpp-file for a description of what they do.

void State::setPolygonMode(PolygonMode m)
{
	_polyMode = m;
	_isPolyModeSet = true;
}
State::PolygonMode State::getPolygonMode()
{
	return _polyMode;
}
bool State::isPolygonModeSet()
{
	return _isPolyModeSet;
}
void State::removePolygonMode()
{
	_polyMode = FILL;
	_isPolyModeSet = false;
}


void State::setShadeType(ShadeType s)
{
	_shadeType = s;
	_isShadeTypeSet = true;
}
State::ShadeType State::getShadeType()
{
	return _shadeType;
}
bool State::isShadeTypeSet()
{
	return _isShadeTypeSet;
}
void State::removeShadeType()
{
	_shadeType = PHONG;
	_isShadeTypeSet = false;
}


void State::setCullFace(bool c)
{
	_cullFace = c;
	_isCullFaceSet = true;
}
bool State::getCullFace()
{
	return _cullFace;
}
bool State::isCullFaceSet()
{
	return _isCullFaceSet;
}
void State::removeCullFace()
{
	_cullFace = true;
	_isCullFaceSet = false;
}

void State::setColorMaterial(bool s)
{
	_colorMaterial = s;
	_isColorMaterialSet = true;
}
bool State::getColorMaterial()
{
	return _colorMaterial;
}
bool State::isColorMaterialSet()
{
	return _isColorMaterialSet;
}
void State::removeColorMaterial()
{
	_colorMaterial = false;
	_isColorMaterialSet = false;
}


void State::setMaterial(Material m)
{	
	_material = m;
	_isMaterialSet = true;
}
Material State::getMaterial()
{
	return _material;
}
void State::removeMaterial()
{
	_material = Material();
	_isMaterialSet = false;
}
bool State::isMaterialSet()
{
	return _isMaterialSet;
}

void State::setShader(shader_ptr s)
{
	_shader = s;
	_isShaderSet = true;
}
shader_ptr State::getShader()
{
	return _shader;
}
bool State::isShaderSet()
{
	return _isShaderSet;
}
void State::removeShader()
{
	_shader = NULL;
	_isShaderSet = false;
}


// Below cumulative states are pushed, poped and inspected etc.

void State::pushLight(Light l)
{
	_lights.push_back(l);	
}	

int State::getNrLights()
{
	return _lights.size();
}
Light State::getLight(int n)
{	
	std::list<Light>::iterator it = _lights.begin();
	std::advance(it,n);
	return *it;
}

bool State::hasTexture(State::TexMap m)
{
	return  _textures.count(m)==1;
}
void State::addTexture(State::TexMap m, Texture t)
{
	_textures.erase(m);
	std::pair<State::TexMap, Texture > pair( m, t );
	_textures.insert(pair);
}
int State::getNrTextures()
{
	return _textures.size();
}

Texture State::getTexture(State::TexMap m)
{
	if(_textures.count(m)){
		return _textures[m];
	}else{
		std::cerr<<"Index out of bounds when accesing a texture in state." << std::endl;
		return Texture();
	}
	
}
void State::popTexture(State::TexMap m)
{
	_textures.erase(m);
}

/*
 * Name:	apply
 * Purpose: Applies the current state to the GL_state, updates
 *			uniforms etc.
 * Input:	-
 * Output:	-
 * Misc:	-
 */
void State::apply()
{
	// Everything hinges on a shader bing set.
	if( isShaderSet() )
	{
		getShader()->use();

		// Set polygon Mode
		if( _polyMode == POINT ){
			glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		}else if( _polyMode == LINE){
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}else if(_polyMode == FILL){
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		
		// Enable or disable backface culling
		glCullFace(GL_FRONT);
		if(_cullFace)
		{
			glEnable(GL_CULL_FACE);
		}else{
			glDisable(GL_CULL_FACE);
		}
		
		// Handle light

		// Count the active lights
		int nrLights = 0;
		for(std::list<Light>::iterator it = _lights.begin(); it!=_lights.end(); it++)
		{
			if(it->isOn())
			{
				nrLights++;	
			}
		}
	
		// Get all the relevant light-material interaction data
		float* lpos = new float[nrLights*3];
		float* diff = new float[nrLights*4];
		float* spec = new float[nrLights*4];
		float* amb = new float[nrLights*4];
		float* att = new float[nrLights];
		int i = 0;
		for(std::list<Light>::iterator it = _lights.begin(); it!=_lights.end(); it++)
		{
			//only use lights that are turned on
 			Light l = *it;
			if(l.isOn())
			{
				float tmp4[4] ={0};
				float tmp3[3] ={0};
				Vec4 ambProd = _material.getAmbient(&l);
				Vec4 diffProd = _material.getDiffuse(&l);
				Vec4 specProd = _material.getSpecular(&l);  

				specProd.get(tmp4);
				spec[4*i+0] = tmp4[0];
				spec[4*i+1] = tmp4[1];
				spec[4*i+2] = tmp4[2];
				spec[4*i+3] = tmp4[3];
			
				diffProd.get(tmp4);
				diff[4*i+0] = tmp4[0];
				diff[4*i+1] = tmp4[1];
				diff[4*i+2] = tmp4[2];
				diff[4*i+3] = tmp4[3];
	
				ambProd.get(tmp4);
				amb[4*i+0] = tmp4[0];
				amb[4*i+1] = tmp4[1];
				amb[4*i+2] = tmp4[2];
				amb[4*i+3] = tmp4[3];

				l.getPosition().get(tmp3);
				lpos[3*i+0] = tmp3[0];
				lpos[3*i+1] = tmp3[1];
				lpos[3*i+2] = tmp3[2];
				
				att[i] = l.getAttenuation();

				i++;
			}
		}
		float shin = _material.getShininess();

		_shader->setUniform4f("specularProduct",nrLights, spec);
		_shader->setUniform4f("diffuseProduct",nrLights, diff);
		_shader->setUniform4f("ambientProduct",nrLights, amb);
		_shader->setUniform3f("lightPosition",nrLights, lpos);
		_shader->setUniform1f("shininess", 1, &shin);
		_shader->setUniform1i("nrLights", 1, &nrLights);
		_shader->setUniform1f("attenuation", nrLights, att);
 	
		delete[] lpos; 
		delete[] diff;
		delete[] spec;
		delete[] amb;
		delete[] att;
	}

}
