#pragma once

#include <glm/glm.hpp>

class Camera
{
	float camTargetDist = 22.f;

	float mouseSensitivity = 0.008f;

	float yaw; // left right
	float pitch; // up down
	glm::vec3 target;

	glm::mat4 transform;
public:

	void update();

};