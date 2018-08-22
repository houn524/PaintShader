#include "Camera.h"
#include <QtDebug>

Camera::Camera(glm::vec3 _position, glm::vec3 up, float _yaw, float _pitch) : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM)
{
	position = _position;
	worldUp = up;
	yaw = _yaw;
	pitch = _pitch;
	updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float _yaw, float _pitch) : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM)
{
	position = glm::vec3(posX, posY, posZ);
	worldUp = glm::vec3(upX, upY, upZ);
	yaw = _yaw;
	pitch = _pitch;
	updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix()
{
	return glm::lookAt(position, position + front, up);
}

void Camera::processKeyboard(CameraMovement direction)//, float deltaTime)
{
	float velocity = movementSpeed;// *deltaTime;
	if (direction == FORWARD)
		position += front * velocity;
	if (direction == BACKWARD)
		position -= front * velocity;
	if (direction == LEFT)
		position -= right * velocity;
	if (direction == RIGHT)
		position += right * velocity;
}

void Camera::processMouseMovement(float xoffset, float yoffset, bool isLeftButton)
{
	xoffset *= mouseSensitivity;
	yoffset *= mouseSensitivity;

	if (isLeftButton)
	{
		position -= right * xoffset * 0.2f;
		position += up * yoffset * 0.2f;
	}
	else if (!isLeftButton)
	{
		yaw += xoffset * 1.5f;
		pitch -= yoffset * 1.5f;
		updateCameraVectors();
	}
}

void Camera::processMouseScroll(float yoffset)
{
	position += front * yoffset;
}

void Camera::updateCameraVectors()
{
	glm::vec3 _front;
	_front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	_front.y = sin(glm::radians(pitch));
	_front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	front = glm::normalize(_front);

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}