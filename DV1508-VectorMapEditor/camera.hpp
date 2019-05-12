#pragma once

#include <glm/glm.hpp>

class Camera
{
	float mouseSensitivity = 0.008f;

	float fov = 60.f;
	float camTargetDist = 22.f;

	float yaw; // left right
	float pitch; // up down
	glm::vec3 target;
	glm::vec3 position;
	glm::mat4 transform;
public:

	void update();

	glm::mat4 getTransform();
	glm::vec3 getPosition();

};