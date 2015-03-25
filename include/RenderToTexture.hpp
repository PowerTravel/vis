#ifndef RENDER_TO_TEXTURE_HPP
#define RENDER_TO_TEXTURE_HPP

//#include <memory>
#include "Group.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
class RenderToTexture: public Group{
	public:
		RenderToTexture(std::shared_ptr<Shader> s, int w=1024, int h = 1024);
		virtual ~RenderToTexture();

		Shader* getShader();

		void acceptVisitor(NodeVisitor& v);

		void bindBuffer();
		void clearBuffer();
		void bindTexture(GLenum id);
		void clearTexture();

		void resizeTexture(int width, int height);

		Texture getTexture();
		
	private:
		GLuint _framebuffer;
		Texture _depthTex;
		std::shared_ptr<Shader> _s;
};

#endif // RENDER_TO_TEXTURE_HPP
