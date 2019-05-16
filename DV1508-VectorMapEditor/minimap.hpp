#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
class Minimap
{
private:
	GLuint fbo;
	GLuint texture;
public:
	Minimap()
	{
		glGenFramebuffers(1, &fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, static_cast<GLsizei>(Window::size().x), static_cast<GLsizei>(Window::size().y), 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	inline void bindFBO()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
	}
	inline void unbindFBO()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	inline GLuint getTextureID()
	{
		return texture;
	}
	~Minimap()
	{
		glDeleteFramebuffers(1, &fbo);
		glDeleteTextures(1, &texture);
	}
};