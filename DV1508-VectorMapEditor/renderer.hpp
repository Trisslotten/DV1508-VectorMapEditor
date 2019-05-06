#pragma once

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include "shader.hpp"

class Renderer
{
public:
	void init();
	void render();

private:
	void initShaders();
	void initMesh();


	ShaderProgram terrainShader;

	int terrainMeshRes = 1024;
	GLuint terrainEBO = 0;
	GLuint terrainVBO = 0;
	GLuint terrainVAO = 0;
	std::vector<glm::vec2> terrainMesh;
	std::vector<uint32_t> indices;
};