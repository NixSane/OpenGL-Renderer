#include "FlyCamera.h"

FlyCamera::FlyCamera(
	const glm::vec3 a_position,
	const glm::vec3 a_look_at_point,
	const float a_aspect_ratio,
	const float a_FOV_Y , // 90 on Y, roughly 120 on X
	const float a_near_distance ,
	const float a_far_distance )
{
	setPerspective(a_FOV_Y, a_aspect_ratio, a_near_distance, a_far_distance);
	setLookAt(a_position, a_look_at_point);
}


void FlyCamera::update(float deltaTime)
{
	// Grab context
	auto glfw_window = glfwGetCurrentContext();
	// Only update camera if input is received
	bool input_flag = false;
	// The build translation vector
	glm::vec3 displacement = glm::vec3(0.0f);

	/* CLASSIC WASD */
	// Forward
	if (glfwGetKey(glfw_window, GLFW_KEY_W))
	{
		displacement -= glm::vec3(worldTransform[2]);
		//displacement -= get_direction();
		input_flag = true;
	}
	// Backward
	if (glfwGetKey(glfw_window, GLFW_KEY_S))
	{
		displacement += glm::vec3(worldTransform[2]);
		input_flag = true;
	}
	// Left
	if (glfwGetKey(glfw_window, GLFW_KEY_A))
	{
		displacement -= glm::vec3(worldTransform[0]);
		input_flag = true;
	}
	// Right
	if (glfwGetKey(glfw_window, GLFW_KEY_D))
	{
		displacement += glm::vec3(worldTransform[0]);
		input_flag = true;
	}
	// Up
	if (glfwGetKey(glfw_window, GLFW_KEY_LEFT_CONTROL))
	{
		displacement -= glm::vec3(0.0f, 1.0f, 0.0f);
		input_flag = true;
	}
	// Down
	if (glfwGetKey(glfw_window, GLFW_KEY_SPACE))
	{
		displacement += glm::vec3(0.0f, 1.0f, 0.0f);
		input_flag = true;
	}

	// Apply the scaled translation to the camera's position, if there was input
	if (input_flag)
	{
		setPosition(worldTransform[3] + (glm::vec4(displacement, 0.0f) * speed * deltaTime));
		updateProjectionViewTransform();
	}

	/* MOUSE LOOK */
	double cursor_position_x;
	double cursor_position_y;
	// Aquire the current cursor position
	glfwGetCursorPos(glfw_window, &cursor_position_x, &cursor_position_y);

	// TODO - STORE Resolution of the camera

	// Calculate the offset from the screens centre this frame
	double delta_x = cursor_position_x - (1280 * 0.5);
	double delta_y = cursor_position_y - (720 * 0.5);
	// Move the cursor back to the centre of the render / window
	// TODO glfwSetInputMode(window , glfw_cursor_disabled)
	// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPos(glfw_window, 1280 * 0.5, 720 * 0.5);

	// Sets delta to zero upon startup
	if (m_first_mousecall)
	{
		delta_x = 0.0f;
		delta_y = 0.0f;
		m_first_mousecall = false;
	}

	// If either input is non-zero, apply the rotation
	if (delta_x || delta_y)
	{
		// Identity matrix to accumulate rotation
		auto rotation = glm::mat4(1.0f);
		// Left / Right rotation
		rotation = glm::rotate(rotation, float(angular_speed * deltaTime * -delta_x), glm::vec3(viewTransform[1]));
		// Up / Down rotation
		rotation = glm::rotate(rotation, float(angular_speed * deltaTime * -delta_y), glm::vec3(1.0f, 0.0f, 0.0f));

		// Apply the rotation to the camera
		worldTransform = worldTransform * rotation;
		// Preserve the inverse
		viewTransform = glm::inverse(worldTransform);
		// Update PxV
		updateProjectionViewTransform();
	}
}