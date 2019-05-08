#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "window.hpp"

class VectorMap
{
public:
	void init(int size = 1024);
	void bind(int slot);
private:
	GLuint texture;

	int size = -1;
	std::vector<glm::vec3> vectorMap;
};