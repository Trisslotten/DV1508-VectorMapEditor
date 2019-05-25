#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "window.hpp"
#include "shader.hpp"

class VectorMap
{
public:
	void init(int size = 1024);
	void bind(int slot);
	void bindAsImage(int slot, GLenum access = GL_READ_WRITE);


	glm::vec2 getSize()
	{
		return glm::vec2(size);
	}
private:
	GLuint texture;
	int size = -1;
	std::vector<glm::vec4> vectorMap;
};