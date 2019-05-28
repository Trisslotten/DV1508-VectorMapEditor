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
#include "texture.hpp"

#define VIEWMODE_SHADED 0
#define VIEWMODE_TEXTURED 1
#define VIEWMODE_NORMALS 2
#define VIEWMODE_WIREFRAME 3
#define VIEWMODE_AUTO 4

class Renderer
{
public:
	void init();
	void createTerrainFramebuffer();
	void render();
	void renderMiniMap(glm::mat4 transform, glm::vec3 camPos);
	void setVectorMap(VectorMap* vmap);
	void setViewMode(int mode);
	void toggleWireFrame();
	void toggleFPSCamera();
	void showBrush(float radius, float strength);

	void showBrushCross(glm::vec2 offset);

	// returns Shader Storage Buffer Object with uv
	GLuint mouseTerrainIntersection();
private:
	void initShaders();
	void initTerrainMesh();

	Camera camera;
	Timer timer;
	bool wireframe = false;
	ShaderProgram terrainShader;
	ShaderProgram texturedShader;
	ShaderProgram normalsShader;
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

	GLuint quadVBO;
	GLuint quadVAO;
	ShaderProgram quadShader;

	ShaderProgram mousePickingShader;
	GLuint uvSSBO = 0;

	Texture brushCross;
	glm::vec2 brushCrossOffset;
	bool showingBrushCross = false;

	float brushRadius = 0.f;
	float brushStrength = 0.f;

	VectorMap* vectorMap = nullptr;
	int viewMode = 0;
};