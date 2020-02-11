#include "FlyCamera.h"

FlyCamera::FlyCamera()
{
	speed = 2.5f;
	up = glm::vec3(0.0f, 1.0f, 0.0f);
}

FlyCamera::FlyCamera(float a_speed) : speed(a_speed) {}

void FlyCamera::setSpeed(float a_speed)
{
	speed = a_speed;
}

void FlyCamera::update(float deltaTime)
{
	GLFWwindow* window = glfwGetCurrentContext();

	// Translating position
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		worldTransform[3].z -= speed * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		worldTransform[3].z += speed * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		worldTransform[3].x += speed * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		worldTransform[3].x += speed * deltaTime;

	// Rotate the camera


	// Update new position and orientation
	updateProjectionViewTransform();

}