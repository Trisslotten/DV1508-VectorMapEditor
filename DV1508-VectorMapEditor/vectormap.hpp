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

	void addHeight(glm::vec2 uv, float radius, float strength);
private:
	GLuint texture;
	int size = -1;
	std::vector<glm::vec4> vectorMap;

	ShaderProgram addHeightShader;
};