#include "Camera.h"

#include <Core\Engine.h>

using namespace std;

Camera::Camera()
{
	position = glm::vec3(0, 2, 5);
	forward = glm::vec3(0, 0, -1);
	up = glm::vec3(0, 1, 0);
	right = glm::vec3(1, 0, 0);
	distanceToTarget = 2;
}

Camera::Camera(const glm::vec3 &position, const glm::vec3 &center, const glm::vec3 &up)
{
	Set(position, center, up);
}

Camera::~Camera()
{
}

void Camera::Set(const glm::vec3 &position, const glm::vec3 &center, const glm::vec3 &up)
{
	this->position = position;
	forward = glm::normalize(center - position);
	right = glm::cross(forward, up);
	this->up = glm::cross(right, forward);
}

void Camera::MoveForward(float distance)
{
	glm::vec3 dir = glm::normalize(glm::vec3(forward.x, 0, forward.z));
	position = position + glm::normalize(dir) * distance;
	// movement will keep the camera at the same height always
	// Example: If you rotate up/down your head and walk forward you will still keep the same relative distance (height) to the ground!
	// Translate the camera using the DIR vector computed from forward
}

void Camera::TranslateForward(float distance)
{
	position += glm::normalize(forward) * distance;
	// TODO : Translate the camera using the "forward" vector
}

void Camera::TranslateUpword(float distance)
{
	position += up * distance;
	// TODO : Translate the camera using the up vector
}

void Camera::TranslateRight(float distance)
{
	//position += glm::vec3(right.x, 0, right.z)) * distance;
	position += glm::normalize(right) * distance;
	// TODO
	// Translate the camera using the "right" vector
	// Usually translation using camera "right' is not very useful because if the camera is rotated around the "forward" vector 
	// translation over the right direction will have an undesired effect; the camera will get closer or farther from the ground
	// Using the projected right vector (onto the ground plane) makes more sense because we will keep the same distance from the ground plane
}

void Camera::RotateFirstPerson_OX(float angle)
{
	forward = glm::normalize(glm::rotate(glm::mat4(1.f), angle, right) * glm::vec4(forward, 0));
	up = glm::normalize(glm::cross(right, forward));
	// TODO
	// Compute the new "forward" and "up" vectors
	// Attention! Don't forget to normalize the vectors
	// Use glm::rotate()
}

void Camera::RotateFirstPerson_OY(float angle)
{
	forward = glm::normalize(glm::rotate(glm::mat4(1.f), angle, glm::vec3(0, 1, 0)) * glm::vec4(forward, 0));
	right = glm::normalize(glm::rotate(glm::mat4(1.f), angle, glm::vec3(0, 1, 0)) * glm::vec4(right, 0));
	up = glm::normalize(glm::cross(right, forward));
	// TODO
	// Compute the new "forward", "up" and "right" vectors
	// Don't forget to normalize the vectors
	// Use glm::rotate()
}

void Camera::RotateFirstPerson_OZ(float angle)
{
	right = glm::normalize(glm::rotate(glm::mat4(1.f), angle, forward) * glm::vec4(right, 0));
	up = glm::normalize(glm::rotate(glm::mat4(1.f), angle, forward) * glm::vec4(up, 0));
	forward = glm::cross(up, right);
	// TODO
	// Compute the new Right and Up, Forward stays the same
	// Don't forget to normalize the vectors
}

void Camera::RotateThirdPerson_OX(float angle)
{
	TranslateForward(distanceToTarget);
	RotateFirstPerson_OX(angle);
	TranslateForward(-distanceToTarget);
	// TODO
	// Rotate the camera in Third Person mode - OX axis
	// Use distanceToTarget as translation distance
}

void Camera::RotateThirdPerson_OY(float angle)
{
	TranslateForward(distanceToTarget);
	RotateFirstPerson_OY(angle);
	TranslateForward(-distanceToTarget);
	// TODO
	// Rotate the camera in Third Person mode - OY axis
}

void Camera::RotateThirdPerson_OZ(float angle)
{
	TranslateForward(distanceToTarget);
	RotateFirstPerson_OZ(angle);
	TranslateForward(-distanceToTarget);
	// TODO
	// Rotate the camera in Third Person mode - OZ axis
}

glm::mat4 Camera::GetViewMatrix()
{
	// Returns the View Matrix
	return glm::lookAt(position, position + forward, up);
}

glm::vec3 Camera::GetTargetPosition()
{
	return position + forward * distanceToTarget;
}
