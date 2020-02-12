#include "Camera.h"

#pragma once
#ifndef _FLY_CAMERA_H_
#define _FLY_CAMERA_H_



class FlyCamera : public Camera
{
public:
	FlyCamera(
		const glm::vec3 a_position,
		const glm::vec3 a_look_at_point,
		const float a_aspect_ratio = 16.0 / 9.0f,
		const float a_FOV_Y = 3.14159f * 0.5f, // 90 on Y, roughly 120 on X
		const float a_near_distance = 0.01f,
		const float a_far_distance = 100.0f);
	

		void update(float deltaTime) override;
	

private:
	float speed = 8.0f; // For movement speed
	float angular_speed = 0.01f; // Mouse motion rotation
	glm::vec3 up = glm::vec3(0.0f, 0.1f, 0.0f);

	bool m_first_mousecall = true;
};
#endif // ! _FLY_CAMERA_H_

