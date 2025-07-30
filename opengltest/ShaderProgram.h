#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>

class ShaderProgram {
public:
	GLuint ID;
	
	ShaderProgram(const char* vertexPath, const char* fragmentPath);
	void use();
	void destruct();
	void setBool(const std::string& name, bool value);
	void setInt(const std::string& name, int value);
	void setFloat(const std::string& name, float value);
	void setMat4(const std::string& name, glm::mat4 value);
	void setVec3(const std::string& name, glm::vec3 value);
	void setVec4(const std::string& name, glm::vec4 value);
	void compileShaders(const char* vertSource, const char* fragSource, unsigned int& shaderRef);
	std::string readFile(const std::string filePath);
	
};

#endif