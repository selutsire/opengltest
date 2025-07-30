#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ShaderProgram.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Texture.h"
#include "Camera.h"

void processInput(GLFWwindow* window, Camera& Camera);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//SCREEN DIMENSIONS
int width = 1920;
int height = 1080;
//DELTA VARIABLE
float delta = 0.0f;
bool changeCamSize = false;
int main(){
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(width, height, "opengltest", NULL, NULL);
	if (window == NULL) {
		std::cout << "failed to create glfw window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	
	
	//SETTING UP VERTEX DATA
	float vertices[] = {
		//     COORDINATES     /        COLORS        /    TexCoord    /       NORMALS     //
	   -1.0f, 0.0f,  1.0f,		0.0f, 0.0f, 0.0f,		0.0f, 0.0f,		0.0f, 1.0f, 0.0f,
	   -1.0f, 0.0f, -1.0f,		0.0f, 0.0f, 0.0f,		0.0f, 1.0f,		0.0f, 1.0f, 0.0f,
		1.0f, 0.0f, -1.0f,		0.0f, 0.0f, 0.0f,		1.0f, 1.0f,		0.0f, 1.0f, 0.0f,
		1.0f, 0.0f,  1.0f,		0.0f, 0.0f, 0.0f,		1.0f, 0.0f,		0.0f, 1.0f, 0.0f
	};
	GLuint indices[] = {
		0, 1, 2,
		0, 2, 3
	};

	GLfloat lightVertices[] =
	{ //     COORDINATES     //
		-0.1f, -0.1f,  0.1f,
		-0.1f, -0.1f, -0.1f,
		 0.1f, -0.1f, -0.1f,
		 0.1f, -0.1f,  0.1f,
		-0.1f,  0.1f,  0.1f,
		-0.1f,  0.1f, -0.1f,
		 0.1f,  0.1f, -0.1f,
		 0.1f,  0.1f,  0.1f
	};

	GLuint lightIndices[] =
	{
		0, 1, 2,
		0, 2, 3,
		0, 4, 7,
		0, 7, 3,
		3, 7, 6,
		3, 6, 2,
		2, 6, 5,
		2, 5, 1,
		1, 5, 4,
		1, 4, 0,
		4, 5, 6,
		4, 6, 7
	};
	
	ShaderProgram shaderProgram("default.vert", "default.frag");
	VAO VAO1;
	VAO1.bind();
	VBO VBO1(vertices, sizeof(vertices));
	EBO EBO1(indices, sizeof(indices));
	VAO1.linkAttrib(VBO1, 0, 3, GL_FLOAT, 11 * sizeof(float), (void*)0);
	VAO1.linkAttrib(VBO1, 1, 3, GL_FLOAT, 11 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.linkAttrib(VBO1, 2, 2, GL_FLOAT, 11 * sizeof(float), (void*)(6 * sizeof(float)));
	VAO1.linkAttrib(VBO1, 3, 3, GL_FLOAT, 11 * sizeof(float), (void*)(8 * sizeof(float)));
	VAO1.unbind();
	VBO1.unbind();
	EBO1.unbind();

	ShaderProgram lightShader("light.vert", "light.frag");
	VAO lightVAO;
	lightVAO.bind();
	VBO lightVBO(lightVertices, sizeof(lightVertices));
	EBO lightEBO(lightIndices, sizeof(lightIndices));
	lightVAO.linkAttrib(lightVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
	lightVAO.unbind();
	lightVBO.unbind();
	lightEBO.unbind();

	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	glm::vec3 pyramidPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 pyramidModel = glm::mat4(1.0f);
	pyramidModel = glm::translate(pyramidModel, pyramidPos);

	lightShader.use();
	lightShader.setMat4("modelMatrix", lightModel);
	lightShader.setVec4("lightColor", lightColor);

	shaderProgram.use();
	shaderProgram.setMat4("modelMatrix", pyramidModel);
	shaderProgram.setVec4("lightColor", lightColor);
	shaderProgram.setVec3("lightPos", lightPos);

	Texture planksTex("planks.png", GL_TEXTURE_2D, 0, GL_UNSIGNED_BYTE);
	planksTex.texUnit(shaderProgram, "tex0", 0);
	Texture planksSpec("planksSpec.png", GL_TEXTURE_2D, 1, GL_UNSIGNED_BYTE);
	planksSpec.texUnit(shaderProgram, "spec0", 1);	
	
	Camera camera(width, height, glm::vec3(0.0f, 0.3f, 0.9f));
	
	float lastFrame = 0.0f;
	
	glEnable(GL_DEPTH_TEST);
	
	//RENDER LOOP
	while (!glfwWindowShouldClose(window)) {
		
		float currentFrame = static_cast<float>(glfwGetTime());
		delta = currentFrame - lastFrame;
		lastFrame = currentFrame;
		if (changeCamSize) {
			camera.updateScreenSize(width, height);
			changeCamSize = false;
		}
		
		//draw background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//process input
		processInput(window, camera);
		//get the camera matrix so the triangles distort properly and set it in the shader
		glm::mat4 camMatrix = camera.matrix();
		
		//DRAW THE PYRAMID
		shaderProgram.use();
		shaderProgram.setVec3("camPos", camera.getPosition());
		shaderProgram.setMat4("camMatrix", camMatrix);
		//bind the pyramid VAO and texture
		planksTex.bind();
		planksSpec.bind();
		VAO1.bind();
		//finally draw the pyramid
		glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(GLuint), GL_UNSIGNED_INT, 0);
		
		//DRAW THE LIGHT CUBE
		lightShader.use();
		lightShader.setMat4("camMatrix", camMatrix);
		lightVAO.bind();
		glDrawElements(GL_TRIANGLES, sizeof(lightIndices) / sizeof(GLuint), GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);	
		glfwPollEvents();
	}

	//delete object created
	VAO1.destruct();
	VBO1.destruct();
	EBO1.destruct();
	planksTex.destruct();
	shaderProgram.destruct();
	lightVAO.destruct();
	lightVBO.destruct();
	lightEBO.destruct();
	lightShader.destruct();
		
	//destroy window
	glfwDestroyWindow(window);
	glfwTerminate();
	
	return 0;
}

void processInput(GLFWwindow* window, Camera& camera) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.processKeyboardInput(CAM_FORWARD, delta);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.processKeyboardInput(CAM_LEFT, delta);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.processKeyboardInput(CAM_BACKWARD, delta);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.processKeyboardInput(CAM_RIGHT, delta);
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		camera.processKeyboardInput(CAM_DOWN, delta);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera.processKeyboardInput(CAM_UP, delta);
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL)
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		double mouseX, mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);
		camera.processMouseMovement((float)mouseX, (float)mouseY);
		glfwSetCursorPos(window, width / 2, height / 2);
	} else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
		
}

void framebuffer_size_callback(GLFWwindow* window, int newWidth, int newHeight)
{
	width = newWidth;
	height = newHeight;
	changeCamSize = true;
	glViewport(0, 0, width, height);
}


