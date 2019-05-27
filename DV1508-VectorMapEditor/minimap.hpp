#pragma once
#include <glm/common.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
class Minimap
{
private:
	GLuint fbo, texture, depthrenderbuffer;
	glm::mat4 view;
public:
	Minimap()
	{
		glGenFramebuffers(1, &fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, static_cast<GLsizei>(Window::size().x), static_cast<GLsizei>(Window::size().y), 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
		glGenRenderbuffers(1, &depthrenderbuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, static_cast<GLsizei>(Window::size().x), static_cast<GLsizei>(Window::size().y));
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);
		GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, DrawBuffers);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//Hack-af
		view[0][0] = -1.86265e-09;
		view[0][1] = 0.995004;
		view[0][2] = 0.0998334;
		view[0][3] = 0;
		view[1][0] = 0;
		view[1][1] = 0.0998334;
		view[1][2] = 0.995004;
		view[1][3] = 0;
		view[2][0] = 1;
		view[2][1] = 1.85334e-09;
		view[2][2] = -1.85944e-10;
		view[2][3] = 0;
		view[3][0] = -0;
		view[3][1] = 2.98023e-08;
		view[3][2] = -3;
		view[3][3] = 1;
	}
	glm::vec3 getPosition()
	{
		return view[3];
	}
	inline glm::mat4 getTransform()
	{
		auto ws = Window::size();
		glm::mat4 perspective = glm::perspective(glm::radians(60.f), 300.f / 300.f, 0.01f, 100.f);
		return perspective * view;
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