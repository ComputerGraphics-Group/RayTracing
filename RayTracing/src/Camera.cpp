
#include "Camera.h"

/// <summary>
/// Intialization function for a camera object.
/// </summary>
/// <param name="position">Current position of the camera</param>
/// <param name="up">The up vector of the camera</param>
Camera::Camera(vec3 position = vec3(0, 0, 0), vec3 up = vec3(0, 1, 0)) {
	Position = position;
	WorldUp = up;
	Yaw = yaw;
	Pitch = pitch;
	Roll = roll;
	MovementSpeed = speed;
	MouseSensitivity = sensitivity;
	Zoom = zoom;
	IsPanning = false;
	updateCameraVectors();
}

/// <summary>
/// Provide the matrix representing the view matrix of the camera
/// </summary>
/// <returns>a mat4 type matrix that contains the area the camera is currently looking at</returns>
mat4 Camera::GetViewMatrix() {
	return lookAt(Position, Position + Front, Up);
}

/// <summary>
/// Processes keyboard presses into camera movement
/// </summary>
/// <param name="direction">the direction of the movement</param>
/// <param name="dT">the time between each frame</param>
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

/// <summary>
/// Processes mouse movement into camera movement
/// </summary>
/// <param name="dx">the yaw movement of the camera</param>
/// <param name="dy">the pitch movement of the camera</param>
/// <param name="dz">the roll movement of the camera</param>
/// <param name="constrainPitch">do I constrain the pitch between -90 to 90 degrees?</param>
void Camera::ProcessMouseMovement(float dx, float dy, float dz, GLboolean constrainPitch) {
	dx *= MouseSensitivity;
	dy *= MouseSensitivity;
	dz *= MouseSensitivity;

	Yaw += dx;
	Pitch += dy;
	Roll += dz;

	if (constrainPitch) {
		if (Pitch > 89.0f) Pitch = 89.0f;
		if (Pitch < -89.0f) Pitch = -89.0f;
	}

	updateCameraVectors();
}

/// <summary>
/// Processes mouse scroll into camera movement
/// </summary>
/// <param name="dy">the amount of scroll</param>
void Camera::ProcessMouseScroll(float dy) {
	Zoom -= dy;
	if (Zoom < 1.0f) Zoom = 1.0f;
	if (Zoom > 45.0f) Zoom = 45.0f;
}

/// <summary>
/// Processes mouse pan into camera movement
/// </summary>
/// <param name="dx">the x movement of the mouse</param>
/// <param name="dy">the y movement of the mouse</param>
void Camera::ProcessMousePan(float dx, float dy) {
	Position -= Right * dx;
	Position -= Up * dy;
}

/// <summary>
/// debug function to return the current position of the camera
/// </summary>
void Camera::SendPosition() {
	std::cout << "(" << Position.x << "," << Position.y << "," << Position.z << ")" << std::endl;
}

/// <summary>
/// driver function of a camera object to update all its variables based on the current rotation and position variables
/// </summary>
void Camera::updateCameraVectors() {
	vec3 front;
	front.x = (cos(radians(Yaw)) * cos(radians(Pitch))) * sin(radians(Roll)) + sin(radians(Pitch)) * cos(radians(Roll));
	front.y = -1 * (cos(radians(Yaw)) * cos(radians(Pitch))) * cos(radians(Roll)) + sin(radians(Pitch)) * sin(radians(Roll));
	front.z = sin(radians(Yaw)) * cos(radians(Pitch));
	Front = normalize(front);

	vec3 newUp;
	newUp.x = cos(radians(Roll));
	newUp.y = sin(radians(Roll));
	newUp.z = 0.0f;
	newUp = normalize(newUp);

	Right = normalize(cross(Front, newUp));
	Up = normalize(cross(Right, Front));
}