#include "Camera.h"

Camera::Camera() 
{
	setPerspective(90.0f, 16.0f / 9.0f, 0.2f, 50.0f);
	setLookAt(glm::vec3(0.0, 0.0, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
};

Camera::Camera(glm::vec3 position, float fieldOfView, float aspectRatio, float near, float far)
{
	setPosition(position);
	setPerspective(fieldOfView, aspectRatio, near, far);
	setLookAt(position, glm::vec3(0.0, 0.0f, 0.0f));
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
	updateProjectionViewTransform();
}

void Camera::setPosition(glm::vec3 position)
{
	worldTransform[3] = glm::vec4(position,1);
	viewTransform = glm::inverse(worldTransform);
	updateProjectionViewTransform();
}

void Camera::update(float deltatime)
{
	
}

void Camera::setLookAt(glm::vec3 from, glm::vec3 to)
{
	viewTransform = glm::lookAt(from, to, glm::vec3(0.0f,1.0f,0.0f));
	worldTransform = glm::inverse(viewTransform);
	updateProjectionViewTransform();
}

void Camera::updateProjectionViewTransform()
{
	viewTransform = glm::inverse(worldTransform);
	projectionViewTransform = getProjection() * getView();
}