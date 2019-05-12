#include "camera.hpp"

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

	glm::vec3 cam_pos = yaw_transform*pitch_transform*glm::vec4(camTargetDist*glm::vec3(-1, 0, 0), 1);

	transform = glm::lookAt(
		cam_pos,
		target,
		glm::vec3(0, 1, 0)
	);
}
