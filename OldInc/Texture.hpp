#ifndef TEXTURE_CPP
#define TEXTURE_CPP

#include <GL/glew.h>
#include <string>
struct aiTexture;

/*
 * Class: 	Texture
 * Purpose: Loads textures to the gpu.
 * Misc:	Only supports 2D textures
 */
class Texture{
	
	public:
		
		Texture();
		Texture( GLuint id);
		Texture( std::string& filename);
		virtual ~Texture();

		void clear();
		void bind(GLenum textureUnit);
		bool loaded();

	private:
		GLuint _id;
		bool _loaded;
};

#endif // TEXTURE_CPP
