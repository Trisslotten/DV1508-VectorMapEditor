#pragma once

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include "shader.hpp"
#include "timer.hpp"

#include "vectormap.hpp"

class Renderer
{
public:
	void init();
	void render();

	void setVectorMap(VectorMap* vmap);
	void toggleWireFrame();
private:
	void initShaders();
	void initMesh();

	Timer timer;
	bool wireframe = false;
	ShaderProgram terrainShader;
	int terrainMeshRes = 1024;
	GLuint terrainEBO = 0;
	GLuint terrainVBO = 0;
	GLuint terrainVAO = 0;
	std::vector<glm::vec2> terrainMesh;
	std::vector<uint32_t> indices;

	VectorMap* vectorMap = nullptr;
};