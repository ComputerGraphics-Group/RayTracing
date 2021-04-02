#pragma once

#include<glad/glad.h>
#include "external/glm/glm.hpp"
#include "external/glm/gtc/matrix_transform.hpp"

#include<iostream>

using namespace glm;

enum class Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

const float speed = 1000.0f;
const float yaw = -90.0f;
const float pitch = 0.0f;
const float roll = 90.0f;
const float sensitivity = 0.1f;
const float zoom = 45.0f;

class Camera {
public:
	vec3 Position;
	vec3 Front;
	vec3 Up;
	vec3 Right;
	vec3 WorldUp;

	float Yaw;
	float Pitch;
	float Roll;

	float MovementSpeed;
	float MouseSensitivity;
	float PanSensitivity;
	float Zoom;

	bool IsPanning;

	Camera(vec3 position, vec3 up);

	mat4 GetViewMatrix();

	void ProcessKeyboard(Camera_Movement direction, float dT);

	void ProcessMouseMovement(float dx, float dy, float dz, GLboolean constrainPitch = true);

	void ProcessMouseScroll(float dy);

	void ProcessMousePan(float dx, float dy);

	void SendPosition();

private:

	void updateCameraVectors();

};