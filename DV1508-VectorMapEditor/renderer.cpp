#include "renderer.hpp"

#include "input.hpp"
#include "window.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "imgui/imgui.h"

void Renderer::init()
{
	initShaders();
	initTerrainMesh();
	createTerrainFramebuffer();


	brushCross.loadTexture("assets/super.png");


	glm::vec2 quadVerts[] = {
		glm::vec2(-1,-1),
		glm::vec2(1,-1),
		glm::vec2(1,1),
		glm::vec2(-1,1),
	};

	glGenVertexArrays(1, &quadVAO);
	glBindVertexArray(quadVAO);

	glGenBuffers(1, &quadVBO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(glm::vec2), quadVerts, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (GLvoid*)0);

	glBindVertexArray(0);


	glGenBuffers(1, &uvSSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, uvSSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::vec2), 0, GL_DYNAMIC_COPY);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, uvSSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void Renderer::createTerrainFramebuffer()
{
	glDeleteTextures(1, &terrainColorTex);
	glDeleteTextures(1, &terrainUVTex);
	glDeleteRenderbuffers(1, &depthRB);
	glDeleteFramebuffers(1, &terrainFB);


	glGenFramebuffers(1, &terrainFB);
	glBindFramebuffer(GL_FRAMEBUFFER, terrainFB);

	auto size = Window::size();

	glGenTextures(1, &terrainColorTex);
	glGenTextures(1, &terrainUVTex);

	glBindTexture(GL_TEXTURE_2D, terrainColorTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, terrainUVTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RG32F, size.x, size.y, 0, GL_RG, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glGenRenderbuffers(1, &depthRB);
	glBindRenderbuffer(GL_RENDERBUFFER, depthRB);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, size.x, size.y);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRB);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, terrainColorTex, 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, terrainUVTex, 0);

	GLenum DrawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, DrawBuffers);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR: Could not create terrain framebuffer\n";
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::render()
{
	if (Window::hasResized())
	{
		auto wsize = Window::size();
		createTerrainFramebuffer();
		std::cout << "Recreating framebuffers: " << (int)wsize.x << "x" << (int)wsize.y << "\n";
	}

	camera.update();

	glm::mat4 cam = camera.getTransform();

	terrainShader.use();
	terrainShader.uniform("camTransform", cam);
	terrainShader.uniform("camPos", camera.getPosition());
	terrainShader.uniform("vectorMap", 1);
	terrainShader.uniform("brushRadius", brushRadius);
	terrainShader.uniform("brushStrength", brushStrength);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, uvSSBO);

	if (vectorMap)
	{
		vectorMap->bind(1);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, terrainFB);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::vec4 uvClearColor{ -1,-1,0,1 };
	glClearBufferfv(GL_COLOR, 1, (float*)&uvClearColor);

	glBindVertexArray(terrainVAO);
	int x = terrainMeshRes - 1;
	int numTris = 6*x*x;
	glDrawElements((wireframe == false) ? GL_TRIANGLES : GL_LINES, numTris, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	brushRadius = 0;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, terrainColorTex);

	quadShader.use();
	quadShader.uniform("aspect", 1.f);
	quadShader.uniform("scale", glm::vec2(1.f));
	quadShader.uniform("offset", glm::vec2(0));
	quadShader.uniform("tex", 0);

	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glBindTexture(GL_TEXTURE_2D, 0);

	if (showingBrushCross)
	{
		brushCross.bind(0);

		auto wsize = Window::size();
		quadShader.uniform("aspect", wsize.x / wsize.y);
		quadShader.uniform("scale", 0.3f*glm::vec2(1, -1));
		quadShader.uniform("offset", brushCrossOffset);

		glDepthFunc(GL_ALWAYS);
		glDisable(GL_CULL_FACE);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		glEnable(GL_CULL_FACE);
		glDepthFunc(GL_LESS);
	}
	showingBrushCross = false;

	glBindVertexArray(0);
}

void Renderer::renderMiniMap()
{

	glm::mat4 cam = camera.getTransform();

	terrainShader.use();
	terrainShader.uniform("camTransform", cam);
	terrainShader.uniform("mouseUV", glm::vec2(-100));
	terrainShader.uniform("camPos", camera.getPosition());
	terrainShader.uniform("vectorMap", 1);

	if (vectorMap)
	{
		vectorMap->bind(1);
	}

	glBindVertexArray(terrainVAO);
	int x = terrainMeshRes - 1;
	int numTris = 6 * x*x;
	glDrawElements(GL_TRIANGLES, numTris, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Renderer::setVectorMap(VectorMap * vmap)
{
	this->vectorMap = vmap;
}

void Renderer::toggleWireFrame()
{
	wireframe = (wireframe == false) ? true : false;
}

void Renderer::showBrush(float radius, float strength)
{
	brushRadius = radius;
	brushStrength = strength;
}

void Renderer::showBrushCross(glm::vec2 offset)
{
	brushCrossOffset = offset;
	showingBrushCross = true;
}

GLuint Renderer::mouseTerrainIntersection()
{
	/*
	glm::mat4 invCam = camera.getInverse();
	glm::vec2 mousePos = Input::mousePosition();
	glm::vec2 ndcMousePos = 2.f * mousePos / Window::size() - 1.f;
	ndcMousePos.y *= -1.f;
	glm::vec4 near = invCam * glm::vec4(ndcMousePos, 0, 1);
	glm::vec4 far = invCam * glm::vec4(ndcMousePos, 1, 1);

	near /= near.w;
	far /= far.w;

	glm::vec3 origin = near;
	glm::vec3 dir = far - near;

	float t = -origin.y / dir.y;
	glm::vec3 intersection = t * dir + origin;

	glm::vec2 result = (glm::vec2(intersection.x, intersection.z) + 1.f) / 2.f;
	*/

	glBindImageTexture(2, terrainUVTex, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RG32F);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, uvSSBO);

	glm::vec2 mousePos = Input::mousePosition();
	mousePos.y = Window::size().y - mousePos.y;

	mousePickingShader.use();
	mousePickingShader.uniform("mousePos", mousePos);

	glDispatchCompute(1, 1, 1);

	glMemoryBarrier(GL_ALL_BARRIER_BITS);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	/*
	glm::vec2 uv;
	glm::vec2 *ptr = (glm::vec2*)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
	uv = *ptr;
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

	std::cout << uv.x << ", " << uv.y << "\n";
	*/


	return uvSSBO;
}


void Renderer::initShaders()
{
	terrainShader.add("terrain.vert");
	terrainShader.add("terrain.frag");
	terrainShader.compile();

	quadShader.add("quad.vert");
	quadShader.add("quad.frag");
	quadShader.compile();

	mousePickingShader.add("mousepicking.comp");
	mousePickingShader.compile();
}

void Renderer::initTerrainMesh()
{
	terrainMesh.reserve(terrainMeshRes*terrainMeshRes);
	for (int y = 0; y < terrainMeshRes; y++)
	{
		for (int x = 0; x < terrainMeshRes; x++)
		{
			// terrainMeshRes-1 so vertices can have uv 1.0
			glm::vec2 uv = glm::vec2(x, y) / (terrainMeshRes - 1.f);
			terrainMesh.push_back(uv);
		}
	}

	auto index = [this](int x, int y)
	{
		return x + y * terrainMeshRes;
	};
	indices.reserve(terrainMeshRes*terrainMeshRes * 6);
	for (int y = 0; y < terrainMeshRes - 1; y++)
	{
		for (int x = 0; x < terrainMeshRes - 1; x++)
		{
			indices.push_back(index(x, y));
			indices.push_back(index(x, y + 1));
			indices.push_back(index(x + 1, y + 1));

			indices.push_back(index(x, y));
			indices.push_back(index(x + 1, y + 1));
			indices.push_back(index(x + 1, y));
		}
	}

	glGenVertexArrays(1, &terrainVAO);
	glBindVertexArray(terrainVAO);

	glGenBuffers(1, &terrainVBO);
	glBindBuffer(GL_ARRAY_BUFFER, terrainVBO);
	glBufferData(GL_ARRAY_BUFFER, terrainMesh.size() * sizeof(glm::vec2), terrainMesh.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &terrainEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrainEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (GLvoid*)0);

	glBindVertexArray(0);
}
