#include "Camera.h"

Camera::Camera(vec3 position = vec3(0, 0, 0), vec3 up = vec3(0, 1, 0)) {
	Position = position;
	WorldUp = up;
	Yaw = yaw;
	Pitch = pitch;
	MovementSpeed = speed;
	MouseSensitivity = sensitivity;
	Zoom = zoom;
	IsPanning = false;
	updateCameraVectors();
}

mat4 Camera::GetViewMatrix() {
	return lookAt(Position, Position + Front, Up);
}

void Camera::ProcessKeyboard(Camera_Movement direction, float dT) {
	float vel = MovementSpeed * dT;
	if (direction == Camera_Movement::FORWARD)
		Position += Front * vel;
	if (direction == Camera_Movement::BACKWARD)
		Position -= Front * vel;
	if (direction == Camera_Movement::LEFT)
		Position -= Right * vel;
	if (direction == Camera_Movement::RIGHT)
		Position += Right * vel;
}

void Camera::ProcessMouseMovement(float dx, float dy, GLboolean constrainPitch) {
	dx *= MouseSensitivity;
	dy *= MouseSensitivity;

	Yaw += dx;
	Pitch += dy;

	if (constrainPitch) {
		if (Pitch > 89.0f) Pitch = 89.0f;
		if (Pitch < -89.0f) Pitch = -89.0f;
	}

	updateCameraVectors();
}

void Camera::ProcessMouseScroll(float dy) {
	Zoom -= dy;
	if (Zoom < 1.0f) Zoom = 1.0f;
	if (Zoom > 45.0f) Zoom = 45.0f;
}

void Camera::ProcessMousePan(float dx, float dy) {
	Position -= Right * dx;
	Position -= Up * dy;
}

void Camera::SendPosition() {
	std::cout << "(" << Position.x << "," << Position.y << "," << Position.z << ")" << std::endl;
}

void Camera::updateCameraVectors() {
	vec3 front;
	front.x = cos(radians(Yaw)) * cos(radians(Pitch));
	front.y = sin(radians(Pitch));
	front.z = sin(radians(Yaw)) * cos(radians(Pitch));
	Front = normalize(front);

	Right = normalize(cross(Front, WorldUp));
	Up = normalize(cross(Right, Front));
}