#pragma once

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include "shader.hpp"
#include "timer.hpp"
#include "camera.hpp"
#include "vectormap.hpp"
#include "minimap.hpp"
class Renderer
{
public:
	void init();
	void render();
	void renderMiniMap();
	void setVectorMap(VectorMap* vmap);
	void toggleWireFrame();

	void showBrush(glm::vec2 uv, float radius);
	glm::vec2 mouseTerrainIntersection();
private:
	void initShaders();
	void initMesh();
	Camera camera;
	Timer timer;
	bool wireframe = false;
	ShaderProgram terrainShader;
	int terrainMeshRes = 1024;
	GLuint terrainEBO = 0;
	GLuint terrainVBO = 0;
	GLuint terrainVAO = 0;
	std::vector<glm::vec2> terrainMesh;
	std::vector<uint32_t> indices;

	glm::vec2 brushUV;
	float brushRadius = 0.f;

	VectorMap* vectorMap = nullptr;
};