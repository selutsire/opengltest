#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ShaderProgram.h"

enum CAMERA_MOVEMENT {
	CAM_FORWARD,
	CAM_BACKWARD,
	CAM_LEFT,
	CAM_RIGHT,
	CAM_DOWN,
	CAM_UP
};

const float YAW_DEFAULT = -90.0f;
const float PITCH_DEFAULT = 0.0f;
const float SPEED_DEFAULT = 2.5;
const float SENSITIVITY_DEFAULT = 0.1f;
const float FOV_DEFAULT = 45.0f;

class Camera {
public:
	//camera attributes
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;
	//euler angles
	float yaw;
	float pitch;
	//camera options
	float movementSpeed;
	float mouseSensitivity;
	float fov;
	//screen width/height
	GLuint width;
	GLuint height;
	//input stuff
	bool firstClick = true;
	float lastX;
	float lastY;

	//constructor
	Camera(GLuint screenWidth, GLuint screenHeight, glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 front = glm::vec3(0.0f, 1.0f, -1.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f));
	//input functions
	void processKeyboardInput(CAMERA_MOVEMENT direction, float delta);
	void processMouseMovement(float newX, float newY, GLboolean constrainPitch = true);
	void processMouseScroll(float yOffset);
	void updateScreenSize(GLuint newWidth, GLuint newHeight);
	glm::mat4 matrix();
	glm::vec3 getPosition() const;
private:
	void updateCamera();
	void resetMouseOffset();
};

#endif