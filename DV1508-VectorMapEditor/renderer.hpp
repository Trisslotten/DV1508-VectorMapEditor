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
	void createTerrainFramebuffer();
	void render();
	void renderMiniMap();
	void setVectorMap(VectorMap* vmap);
	void toggleWireFrame();

	void showBrush(float radius);

	// returns Shader Storage Buffer Object with uv
	GLuint mouseTerrainIntersection();
private:
	void initShaders();
	void initTerrainMesh();
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

	GLuint terrainFB = 0;
	GLuint terrainColorTex = 0;
	GLuint terrainUVTex = 0;
	GLuint depthRB = 0;

	GLuint triangleVBO;
	GLuint triangleVAO;
	ShaderProgram triangleShader;

	ShaderProgram mousePickingShader;
	GLuint uvSSBO = 0;

	float brushRadius = 0.f;

	VectorMap* vectorMap = nullptr;
};