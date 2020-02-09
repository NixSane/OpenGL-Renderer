#include "Camera.h"

Camera::Camera() 
{
	m_camera_speed = 3.0f;
	m_cam_pos = glm::vec3(0.0f, 0.0f, 0.0f);
};

Camera::~Camera() {};

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

void Camera::setPerspective(float fieldOfView, float aspectRatio, float near, float far)
{
	
}

void Camera::setPosition(glm::vec3 position)
{
	m_cam_pos = position;
}

void Camera::update(float deltatime)
{
	
}