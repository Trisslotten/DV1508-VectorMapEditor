#include "camera.hpp"

#include <iostream>
#include "window.hpp"
#include "input.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>

void Camera::update()
{
	float scroll = Window::mouseScroll().y;
	float camTDistChange = -scrollSensitivity * scroll * camTargetDist;

	const glm::vec3 up{ 0,1,0 };

	glm::vec2 mouseMov = Input::mouseMovement();
	if (Input::isMouseButtonDown(GLFW_MOUSE_BUTTON_3))
	{
		Window::showCursor(false);
		if (Input::isKeyDown(GLFW_KEY_LEFT_SHIFT))
		{
			glm::vec3 look = normalize(target - position);

			/*
			glm::vec3 side = normalize(cross(up, look));
			glm::vec3 camUp = normalize(cross(look, side));

			target += camTargetDist * panSpeed * ( side * mouseMov.x + camUp * mouseMov.y);
			*/

			look.y = 0;
			glm::vec3 forward = normalize(look);
			glm::vec3 side = normalize(cross(up, forward));
			target += camTargetDist * panSpeed * (side * mouseMov.x + forward * mouseMov.y);
		}
		else
		{
			pitch -= mouseMov.y * mouseSensitivity;
			yaw -= mouseMov.x * mouseSensitivity;
		}
	}
	else
	{
		Window::showCursor(true);
	}

	if (Input::isKeyDown(GLFW_KEY_LEFT_SHIFT))
	{
		target.y -= 0.5f*camTDistChange;
		camTDistChange = 0.f;
	}


	camTargetDist += camTDistChange;
	
	pitch = glm::clamp(pitch, -glm::half_pi<float>() + 0.1f, glm::half_pi<float>() - 0.1f);
	//yaw = glm::mod(yaw, glm::two_pi<float>());

	glm::mat4 pitch_transform = glm::rotate(glm::mat4(), pitch, glm::vec3(0, 0, 1));
	glm::mat4 yaw_transform = glm::rotate(glm::mat4(), yaw, glm::vec3(0, 1, 0));

	position = yaw_transform*pitch_transform*glm::vec4(camTargetDist*glm::vec3(-1, 0, 0), 1);
	position += target;

	transform = glm::lookAt(
		position,
		target,
		up
	);

	auto ws = Window::size();
	transform = glm::perspective(glm::radians(fov), ws.x / ws.y, 0.1f, 100.f) * transform;
}

glm::mat4 Camera::getTransform()
{
	return transform;
}

glm::vec3 Camera::getPosition()
{
	return position;
}
