#include "Camera.h"

Camera::Camera(GLuint screenWidth, GLuint screenHeight, glm::vec3 position, glm::vec3 front, glm::vec3 up) {
	Camera::position =	position;
	Camera::front =		front;
	Camera::worldUp =	up;
	
	yaw =				YAW_DEFAULT;
	pitch =				PITCH_DEFAULT;
	movementSpeed =		SPEED_DEFAULT;
	mouseSensitivity =	SENSITIVITY_DEFAULT;
	fov =				FOV_DEFAULT;
	width = screenWidth;
	height = screenHeight;
	lastX = width / 2;
	lastY = height / 2;
	updateCamera();
}

void Camera::processKeyboardInput(CAMERA_MOVEMENT direction, float delta) {
	float velocity = movementSpeed * delta;
	if (direction == CAM_FORWARD)
		position += front * velocity;
	if (direction == CAM_BACKWARD)
		position -= front * velocity;
	if (direction == CAM_LEFT)
		position -= right * velocity;
	if (direction == CAM_RIGHT)
		position += right * velocity;
	if (direction == CAM_UP)
		position += up * velocity;
	if (direction == CAM_DOWN)
		position -= up * velocity;
}

void Camera::processMouseMovement(float newX, float newY, GLboolean constrainPitch) {
	float  xOffset = lastX - newX;
	xOffset *= mouseSensitivity;
	float yOffset = lastY - newY;
	yOffset *= mouseSensitivity;
	yaw		-= xOffset;
	pitch	+= yOffset;
	resetMouseOffset();

	if (constrainPitch) {
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;
	}

	updateCamera();
}

void Camera::processMouseScroll(float yOffset) {
	fov -= (float)yOffset;
	if (fov < 1.0f)
		fov = 1.0f;
	if (fov > 60.0f)
		fov = 60.0f;
}

void Camera::updateScreenSize(GLuint newWidth, GLuint newHeight) {
	width = newWidth;
	height = newHeight;
}

void Camera::resetMouseOffset() {
	lastX = width / 2;
	lastY = height / 2;
}

glm::mat4 Camera::matrix() {	
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	view = glm::lookAt(position, position + front, up);
	projection = glm::perspective(glm::radians(fov), (float)width / height, 0.1f, 100.0f);

	
	return projection * view;
}

glm::vec3 Camera::getPosition() const {
	return position;
}

void Camera::updateCamera() {
	glm::vec3 newFront;
	newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	newFront.y = sin(glm::radians(pitch));
	newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	
	front = glm::normalize(newFront);
	right = glm::normalize(glm::cross(front, worldUp));
	up =	glm::normalize(glm::cross(right, front));

}