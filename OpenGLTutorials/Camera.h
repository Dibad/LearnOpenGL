#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

enum class Movement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};


class Camera
{
private:

	// Camera attributes
	glm::vec3 position_;
	glm::vec3 front_;
	glm::vec3 up_;
	glm::vec3 right_;
	glm::vec3 worldUp_;

	// Euler angles
	float yaw_; // Y
	float pitch_; // X

	// Camera options
	float speed_;
	float sensitivity_;
	float zoom_;

public:
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = -90.0f, float pitch = 0.0f);
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw = -90.0f, float pitch = 0.0f);

	// Getters

	glm::mat4 getViewMatrix() const;

	float getZoom() const;

	glm::vec3 getPosition() const;

	glm::vec3 getFront() const;

	// process Input

	void processKeyboard(Movement direction, float deltaTime);

	void processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);

	void processMouseScroll(float yoffset);

private:

	void updateCameraVectors();
};

