#include "Camera.h"

#pragma once
class FlyCamera : public Camera
{
public:
	FlyCamera();
	FlyCamera(float speed);

	void update(float deltaTime) override;
	void setSpeed(float a_speed);

private:
	float speed = 0.0f;
	glm::vec3 up = glm::vec3(0.0f, 0.1f, 0.0f);
};

