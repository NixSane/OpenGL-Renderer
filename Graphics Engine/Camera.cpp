#include "Camera.h"

Camera::Camera() 
{
	worldTransform = glm::mat4(1.0f);
	viewTransform = glm::mat4(1.0f);
	projectionTransform = glm::mat4(1.0f);
	projectionViewTransform = glm::mat4(1.0f);
};

Camera::Camera(glm::vec3 position, float fieldOfView, float aspectRatio, float near, float far)
{
	setPosition(position);
	setPerspective(fieldOfView, aspectRatio, near, far);
	projectionViewTransform = projectionTransform * viewTransform;
}

Camera::~Camera() 
{
};

// Returns the Camera's position and orientation in World Space
glm::mat4 Camera::getWorldTransform()
{
	return worldTransform;
}

// Returns the Camera's View's position and orientation
glm::mat4 Camera::getView() 
{
	return viewTransform;
}

// Returns Camera's projection
glm::mat4 Camera::getProjection() 
{
	return projectionTransform;
}

// Camera's projection's view
glm::mat4 Camera::getProjectionView() 
{
	return projectionViewTransform;
}

// Set the camera's field of view
void Camera::setPerspective(float fieldOfView, float aspectRatio, float near, float far)
{
	projectionTransform = glm::perspective(glm::radians(fieldOfView), aspectRatio, near, far);
}

void Camera::setPosition(glm::vec3 position)
{
	worldTransform = glm::translate(worldTransform, position);
}

void Camera::update(float deltatime)
{
	
}

void Camera::setLookAt(glm::vec3 from, glm::vec3 to, glm::vec3 up)
{
	viewTransform = glm::lookAt(from, to, up);
}