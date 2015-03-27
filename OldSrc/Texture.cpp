#include "Texture.hpp"
#include "vr/JPGReader.h"
#include "vr/PNGReader.h"
#include <assimp/scene.h>
#include <iostream>

Texture::Texture()
{
	_loaded = false;
}

Texture::Texture( GLuint id)
{
	_id = id;
	_loaded = true;
}

/*
 * Name:	Texture
 * Purpose: Given a filename and textureTarget (probably redundant as
 *			support only exists for 2D_textures) it can load a 
 *			texture onto the gpu.
 * Input:	-GLenum textureTarget: The type of texture to load.
 * 			-std::string filename: The path to the file to be loaded
 * Output:	-
 * Misc:	Only jpg and png supported
 */
Texture::Texture(std::string& filename)
{
	vr::Image* img = NULL;
	_loaded = false;

	// Load the file.
	if(filename.find(".jpg")!=std::string::npos)
	{
		vr::JPGReader reader = vr::JPGReader();
		img = reader.readImage( filename );

	}else if( filename.find(".png")!=std::string::npos){

		vr::PNGReader reader = vr::PNGReader();
		img = reader.readImage( filename );	
	}else{
		std::cerr << filename << " is not a supported filetype"<< std::endl; 
	}

	// send the texture to the gpu	
	if(img != NULL ){
		glGenTextures(1, &_id);
		glBindTexture(GL_TEXTURE_2D, _id);	
  	
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img->width(), img->height(), 0, GL_RGB, GL_UNSIGNED_BYTE, img->data() );

  		glBindTexture(GL_TEXTURE_2D, 0);

		_loaded = true;
	}
}
Texture::~Texture()
{
	
}

/*
 * Name:	clear
 * Purpose: Unbinds the texture loaded to _textureTarget
 * Input:	-
 * Output:	-
 * Misc:	-
 */
void Texture::clear()
{
	glBindTexture(GL_TEXTURE_2D,0);
}

/*
 * Name:	bind	
 * Purpose: Binds its texture to the glState
 * Input:	GLenum textureUnit: The slot 
 * Output:	-
 * Misc:	-
 */
void Texture::bind(GLenum textureUnit)
{
	if(_loaded)
	{
		glActiveTexture(textureUnit);
		glBindTexture(GL_TEXTURE_2D, _id);
	}
}

bool Texture::loaded()
{
	return _loaded;
}
