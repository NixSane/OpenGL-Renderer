#include "glm.hpp"
#include "glfw3.h"
#include "ext.hpp"

#pragma once
class Camera
{
public:
	Camera();
	~Camera();

	// Getters
	glm::mat4 getWorldTransform();
	glm::mat4 getView();
	glm::mat4 getProjection();
	glm::mat4 getProjectionView();

	// Setters
	void setPerspective(float fieldOfView, float aspectRatio, float near, float far);
	void setLookAt(glm::vec3 from, glm::vec3 to, glm::vec3 up) {};
	void setPosition(glm::vec3 position);
	
	// Update
	void update(float deltatime);

private:
	glm::mat4 worldTransform = glm::mat4();
	glm::mat4 viewTransform = glm::mat4();
	glm::mat4 projectionTransform = glm::mat4();
	glm::mat4 projectionViewTransform = glm::mat4();

	float m_camera_speed = 0;
	glm::vec3 m_cam_pos = glm::vec3();
};

