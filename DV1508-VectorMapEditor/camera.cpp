#pragma once
#include "camera.hpp"

#include <iostream>
#include "window.hpp"
#include "input.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>

#include "imgui/imgui.h"
void Camera::update()
{
	if (fpsCamera)
	{
		auto ws = Window::size();
		int newState = glfwGetMouseButton(Window::getGLFWWindow(), GLFW_MOUSE_BUTTON_RIGHT);
		if (newState == GLFW_RELEASE && camera::oldState == GLFW_PRESS) 
		{
			fpscam.fpsactive = (!fpscam.fpsactive) ? (true) : (false);
		}
		camera::oldState = newState;
		if (fpscam.fpsactive)
		{
			auto size = Window::size();
			
			glfwSetInputMode(Window::getGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			fpscam.forward = glm::normalize(fpscam.pos - fpscam.target);
			glm::vec3 cameraRight = glm::normalize(glm::cross(fpscam.up, fpscam.forward));
			glm::vec3 cameraUp = glm::cross(fpscam.forward, cameraRight);
			float cameraSpeed = 0.05f; // adjust accordingly
			if (glfwGetKey(Window::getGLFWWindow(), GLFW_KEY_W) == GLFW_PRESS)
			{
				fpscam.pos -= cameraSpeed * fpscam.forward;
			}
			if (glfwGetKey(Window::getGLFWWindow(), GLFW_KEY_S) == GLFW_PRESS)
			{
				fpscam.pos += cameraSpeed * fpscam.forward;
			}
			if (glfwGetKey(Window::getGLFWWindow(), GLFW_KEY_A) == GLFW_PRESS)
			{
				fpscam.pos += glm::normalize(glm::cross(fpscam.forward, cameraUp)) * cameraSpeed;
			}
			if (glfwGetKey(Window::getGLFWWindow(), GLFW_KEY_D) == GLFW_PRESS)
			{
				fpscam.pos -= glm::normalize(glm::cross(fpscam.forward, cameraUp)) * cameraSpeed;
			}
			//std::cout << fpscam.forward.x << " : " << fpscam.forward.y << " : " << fpscam.forward.z << std::endl;
			fpscam.target.x = cos(glm::radians(fpscam.pitch)) * cos(glm::radians(fpscam.yaw));
			fpscam.target.y = sin(glm::radians(fpscam.pitch));
			fpscam.target.z = cos(glm::radians(fpscam.pitch)) * sin(glm::radians(fpscam.yaw));
			double xpos, ypos;
			glfwGetCursorPos(Window::getGLFWWindow(), &xpos, &ypos);
			double xoffset = xpos - fpscam.lastx;
			double yoffset = fpscam.lasty - ypos;

			float sensitivity = 0.05f;
			xoffset *= sensitivity;
			yoffset *= sensitivity;
			fpscam.yaw += xoffset;
			fpscam.pitch += yoffset;
			if (fpscam.pitch > 89.0f)
			{
				fpscam.pitch = 89.0f;
			}
			if (fpscam.pitch < -89.0f)
			{
				fpscam.pitch = -89.0f;
			}
			glm::vec3 front;
			front.x = cos(glm::radians(fpscam.pitch)) * cos(glm::radians(fpscam.yaw));
			front.y = sin(glm::radians(fpscam.pitch));
			front.z = cos(glm::radians(fpscam.pitch)) * sin(glm::radians(fpscam.yaw));
			fpscam.forward = glm::normalize(front);
			
			view = glm::lookAt(fpscam.pos, fpscam.pos + fpscam.forward, fpscam.up);
			
			fpscam.lastx = xpos;
			fpscam.lasty = ypos;
			fpscam.lastlook = fpscam.pos + fpscam.forward;
		}
		else
		{
			glfwSetInputMode(Window::getGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			view = glm::lookAt(fpscam.pos, fpscam.lastlook, fpscam.up);
		}
		perspective = glm::perspective(glm::radians(fov), ws.x / ws.y, 0.01f, 100.f);
		transform = perspective * view;
	}
	else
	{
		float scroll = Window::mouseScroll().y;
		float camTDistChange = -scrollSensitivity * scroll * camTargetDist;

		const glm::vec3 up{ 0,1,0 };
		glm::vec2 mouseMov = Input::mouseMovement();
		if (Input::isMouseButtonDown(GLFW_MOUSE_BUTTON_2))
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

		position = yaw_transform * pitch_transform*glm::vec4(camTargetDist*glm::vec3(-1, 0, 0), 1);
		position += target;
		view = glm::lookAt(
			position,
			target,
			up
		);
		
		auto ws = Window::size();
		perspective = glm::perspective(glm::radians(fov), ws.x / ws.y, 0.01f, 100.f);
		transform = perspective * view;
		fpscam.pos = position;
		fpscam.target = fpscam.lastlook = target;
		fpscam.forward = target;
		fpscam.lastx = ws.x / 2.0, fpscam.lasty = ws.y / 2.0;
	}

}

glm::mat4 Camera::getTransform()
{
	return transform;
}

glm::mat4 Camera::getInverse()
{
	return glm::inverse(view)*glm::inverse(perspective);
}

glm::vec3 Camera::getPosition()
{
	return position;
}
