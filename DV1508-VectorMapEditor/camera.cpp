#include "camera.hpp"

#include "window.hpp"
#include "input.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>

void Camera::update()
{
	glm::vec2 delta = Input::mouseMovement();
		
	pitch -= delta.y * mouseSensitivity;
	yaw -= delta.x * mouseSensitivity;
	pitch = glm::clamp(pitch, -glm::half_pi<float>() + 0.1f, glm::half_pi<float>() - 0.1f);
	//yaw = glm::mod(yaw, glm::two_pi<float>());

	glm::mat4 pitch_transform = glm::rotate(glm::mat4(), pitch, glm::vec3(0, 0, 1));
	glm::mat4 yaw_transform = glm::rotate(glm::mat4(), yaw, glm::vec3(0, 1, 0));

	position = yaw_transform*pitch_transform*glm::vec4(camTargetDist*glm::vec3(-1, 0, 0), 1);

	transform = glm::lookAt(
		position,
		target,
		glm::vec3(0, 1, 0)
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
