#pragma once

#include <glm/glm.hpp>

namespace camera
{
	static float camspeed = 0.0f;
	static int camdistance = 0.0;
	static int oldState = 0;
}
class Camera
{
	struct
	{
		bool fpsactive = false;
		glm::vec3 pos;
		glm::vec3 target;
		glm::vec3 forward = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 up = { 0.0, 1.0, 0.0 };
		float yaw = 0.f;
		float pitch = -0.5f;
		double lastx, lasty;
	}fpscam;
	bool fpsCamera = false;
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
	void toggleFPS()
	{
		fpsCamera = (!fpsCamera) ? (true) : (false);
	}
	glm::mat4 getTransform();
	glm::mat4 getInverse();
	glm::vec3 getPosition();

};