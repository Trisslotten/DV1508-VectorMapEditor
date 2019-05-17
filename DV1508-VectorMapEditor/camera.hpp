#pragma once

#include <glm/glm.hpp>

class Camera
{
	float mouseSensitivity = 0.008f;
	float scrollSensitivity = 0.1f;
	float panSpeed = 0.0011111f;

	float fov = 60.f;
	float camTargetDist = 3.f;

	float yaw = 0.f; // left right
	float pitch = -0.5f; // up down
	glm::vec3 target;
	glm::vec3 position;
	glm::mat4 transform;

	glm::mat4 view;
	glm::mat4 perspective;
public:
	void update();

	glm::mat4 getTransform();
	glm::mat4 getInverse();
	glm::vec3 getPosition();

};