#include "RenderToTexture.hpp"
#include "NodeVisitor.hpp"

RenderToTexture::RenderToTexture(std::shared_ptr<Shader> s, int w, int h)
{
	_s = s;
	_framebuffer= 0;
	glGenFramebuffers(1, &_framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);

	GLuint depthTex = 0;
	glGenTextures(1, &depthTex);
	glBindTexture(GL_TEXTURE_2D, depthTex);

    GLint portSize[4];
    glGetIntegerv( GL_VIEWPORT, portSize );
	float ww = (float) portSize[2]-portSize[0];
	float hh = (float) portSize[3]-portSize[1];
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16 , ww, hh, 0, GL_DEPTH_COMPONENT, GL_FLOAT,0  );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTex, 0);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTex, 0);

	// Draw only depth, no color.
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cerr << "Failed to create framebuffer in STATE.cpp" << std::endl;
	}

	_depthTex = Texture(depthTex);

	
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);

}
RenderToTexture::~RenderToTexture()
{

}

void RenderToTexture::resizeTexture(int width, int height)
{
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16 , width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT,0  );
	
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cerr << "Failed to resize framebuffer in STATE.cpp" << std::endl;
	}
}

void RenderToTexture::acceptVisitor(NodeVisitor& v)
{	
	v.apply(this);
}

void RenderToTexture::bindBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cerr << "Failed to create framebuffer in STATE.cpp" << std::endl;
	}
}
void RenderToTexture::clearBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderToTexture::bindTexture(GLenum id)
{
	_depthTex.bind(id);
}
void RenderToTexture::clearTexture()
{
	_depthTex.clear();
}

Shader* RenderToTexture::getShader()
{
	return  _s.get();
}

Texture RenderToTexture::getTexture()
{
	return _depthTex;
}
