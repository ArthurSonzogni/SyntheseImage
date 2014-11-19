#include "Camera.hpp"
#include "system/Input.hpp"
#include <GLFW/glfw3.h>
#include <iostream>
#define THETA_MAX -5*2*M_PI/360
#define THETA_MIN -170*2*M_PI/360
#define R_MIN 0.1

float Camera::THETA_MULTIPLIER = 0.01;
float Camera::PHI_MULTIPLIER = 0.01;
float Camera::ZOOM_MULTIPLIER = 0.1;
float Camera::TRANSLATION_MULTIPLIER = 0.1;

Camera::Camera()
{
	this->r = 10;
	this->theta = -90;
	this->phi = 0;
	this->target = glm::vec3(0.0,0.0,0.0);
}

void Camera::update()
{
	// Manage rotation
	if(Input::isMouseHold(GLFW_MOUSE_BUTTON_LEFT))
	{
		this->theta -= Input::deltaY() * Camera::THETA_MULTIPLIER;
		if(this->theta > THETA_MAX) this->theta = THETA_MAX;
		if(this->theta < THETA_MIN) this->theta = THETA_MIN;
		this->phi += Input::deltaX() * Camera::PHI_MULTIPLIER;
	}
	// Manage translation
	else if(Input::isMouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		this->target += glm::vec3(
				-Input::deltaX()*cos(phi)*Camera::TRANSLATION_MULTIPLIER, 
				-Input::deltaY()*Camera::TRANSLATION_MULTIPLIER, 
				Input::deltaX()*sin(phi)*Camera::TRANSLATION_MULTIPLIER
				);
	}


	// Manage zoom
	if(Input::isKeyHold(GLFW_KEY_KP_ADD) || Input::isKeyHold(GLFW_KEY_LEFT_SHIFT))
	{
		this->r -= Camera::ZOOM_MULTIPLIER;
	}
	if(Input::isKeyHold(GLFW_KEY_KP_SUBTRACT) || Input::isKeyHold(GLFW_KEY_LEFT_CONTROL))
	{
		this->r += Camera::ZOOM_MULTIPLIER;
	}
	if(this->r < R_MIN) this->r = R_MIN;
}

glm::mat4 Camera::lookAt()
{
	return glm::lookAt(
			glm::vec3(r*sin(phi)*sin(theta), r*cos(theta), r*cos(phi)*sin(theta))+target,
			target,
			glm::vec3(0.0, 1.0, 0.0));
}

void Camera::setTarget(glm::vec3 target)
{
	this->target = target;
}

void Camera::setTheta(float theta)
{
	this->theta = theta;
	if(this->theta > THETA_MAX) this->theta = THETA_MAX;
	if(this->theta < THETA_MIN) this->theta = THETA_MIN;
}

void Camera::setPhi(float phi)
{
	this->phi = phi;
}
