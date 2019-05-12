#include "renderer.hpp"

#include "input.hpp"
#include "window.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

void Renderer::init()
{
	initShaders();
	initMesh();
}

void Renderer::render()
{
	camera.update();

	glm::mat4 cam = camera.getTransform();

	terrainShader.use();
	terrainShader.uniform("camTransform", cam);
	terrainShader.uniform("camPos", camera.getPosition());
	terrainShader.uniform("vectorMap", 1);

	if (vectorMap)
	{
		vectorMap->bind(1);
	}

	glBindVertexArray(terrainVAO);
	int x = terrainMeshRes - 1;
	int numTris = 6*x*x;
	glDrawElements((wireframe == false) ? GL_TRIANGLES : GL_LINES, numTris, GL_UNSIGNED_INT, 0);
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


void Renderer::initShaders()
{
	terrainShader.add("terrain.vert");
	terrainShader.add("terrain.frag");
	terrainShader.compile();
}

void Renderer::initMesh()
{
	terrainMesh.reserve(terrainMeshRes*terrainMeshRes);
	for (int y = 0; y < terrainMeshRes; y++)
	{
		for (int x = 0; x < terrainMeshRes; x++)
		{
			// meshRes-1 so some vertices can have uv 1.0
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
