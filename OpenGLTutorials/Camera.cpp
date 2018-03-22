#include "Camera.h"

#include <iostream>


Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) :
	position_(position),
	front_(glm::vec3(0.0f, 0.0f, -1.0f)),
	up_(glm::vec3()),
	right_(glm::vec3()),
	worldUp_(up),
	yaw_(yaw),
	pitch_(pitch),
	speed_(2.5f),
	sensitivity_(0.1f),
	zoom_(45.0f)
{
	updateCameraVectors();
}


Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) :
	Camera(glm::vec3(posX, posY, posZ), glm::vec3(upX, upY, upZ), yaw, pitch)
{
}


glm::mat4 Camera::getViewMatrix() const
{
	return glm::lookAt(position_, position_ + front_, up_);
}


float Camera::getZoom() const
{
	return zoom_;
}


glm::vec3 Camera::getPosition() const
{
	return position_;
}


glm::vec3 Camera::getFront() const
{
	return front_;
}


void Camera::processKeyboard(Movement direction, float deltaTime)
{
	float velocity = speed_ * deltaTime;
	if (direction == Movement::FORWARD)
		position_ += front_ * velocity;
	else if (direction == Movement::BACKWARD)
		position_ -= front_ * velocity;
	else if (direction == Movement::RIGHT)
		position_ += right_ * velocity;
	else if (direction == Movement::LEFT)
		position_ -= right_ * velocity;
	else if (direction == Movement::UP)
		position_ += up_ * velocity;
	else if (direction == Movement::DOWN)
		position_ -= up_ * velocity;
}


void Camera::processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
	yaw_ += xoffset * sensitivity_;
	pitch_ += yoffset * sensitivity_;

	if (constrainPitch)
	{
		if (pitch_ > 89.0f) pitch_ = 89.0f;
		else if (pitch_ < -89.0f) pitch_ = -89.0f;
	}

	// Update front, right and up vectors using the updated euler angles
	updateCameraVectors();
}


void Camera::processMouseScroll(float yoffset)
{
	if (zoom_ >= 1.0f && zoom_ <= 45.0f)
		zoom_ -= yoffset;
	
	else if (zoom_ <= 1.0f)
		zoom_ = 1.0f;

	else if (zoom_ >= 45.0f)
		zoom_ = 45.0f;
}


void Camera::updateCameraVectors()
{
	//Calculate new front vector from the camera's updated Euler Angles
	glm::vec3 nfront;
	nfront.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
	nfront.y = sin(glm::radians(pitch_));
	nfront.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
	front_ = glm::normalize(nfront);
	// Also recalculate right and up vectors
	right_ = glm::normalize(glm::cross(front_, worldUp_));
	up_ = glm::normalize(glm::cross(right_, front_));
}
