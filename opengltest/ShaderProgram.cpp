#include "ShaderProgram.h"

ShaderProgram::ShaderProgram(const char* vertexPath, const char* fragmentPath) {
	std::string vertexCode = readFile(vertexPath);
	std::string fragmentCode = readFile(fragmentPath);
	compileShaders(vertexCode.c_str(), fragmentCode.c_str(), ID);
}

void ShaderProgram::use() {
	glUseProgram(ID);
}

void ShaderProgram::destruct() {
	glDeleteProgram(ID);
}

void ShaderProgram::setBool(const std::string& name, bool value) {
	GLuint location = glGetUniformLocation(ID, name.c_str());
	if (location == -1) {
		std::cerr << "Uniform '" << name << "' not found in shader program." << std::endl;
		return;
	}
	glUniform1i(location, (int)value);
}

void ShaderProgram::setInt(const std::string& name, int value) {
	GLuint location = glGetUniformLocation(ID, name.c_str());
	if (location == -1) {
		std::cerr << "Uniform '" << name << "' not found in shader program." << std::endl;
		return;
	}
	glUniform1i(location, value);
}

void ShaderProgram::setFloat(const std::string& name, float value) {
	GLuint location = glGetUniformLocation(ID, name.c_str());
	if (location == -1) {
		std::cerr << "Uniform '" << name << "' not found in shader program." << std::endl;
		return;
	}
	glUniform1f(location, value);
}

void ShaderProgram::setMat4(const std::string& name, glm::mat4 value) {
	GLuint location = glGetUniformLocation(ID, name.c_str());
	if (location == -1) {
		std::cerr << "Uniform '" << name << "' not found in shader program." << std::endl;
		return;
	}
	glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]);
}

void ShaderProgram::setVec3(const std::string& name, glm::vec3 value) {
	GLuint location = glGetUniformLocation(ID, name.c_str());
	if (location == -1) {
		std::cerr << "Uniform '" << name << "' not found in shader program." << std::endl;
		return;
	}
	glUniform3fv(location, 1, &value[0]);
}

void ShaderProgram::setVec4(const std::string& name, glm::vec4 value) {
	GLuint location = glGetUniformLocation(ID, name.c_str());
	if (location == -1) {
		std::cerr << "Uniform '" << name << "' not found in shader program." << std::endl;
		return;
	}
	glUniform4fv(location, 1, &value[0]);
}

void ShaderProgram::compileShaders(const char* vertSource, const char* fragSource, unsigned int& shaderRef) {
	//CREATE AND COMPILE VERTEX SHADER
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertSource, NULL);
	glCompileShader(vertexShader);

	//ERROR HANDLING FOR VERTEX SHADER
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, sizeof(infoLog), NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED" << infoLog << std::endl;
	}

	//CREATE AND COMPILE FRAGMENT SHADER
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragSource, NULL);
	glCompileShader(fragmentShader);

	//ERROR HANDLING FOR FRAGMENT SHADER
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, sizeof(infoLog), NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//CREATE AND LINK SHADER PROGRAM WITH ALL SHADERS
	shaderRef = glCreateProgram();
	glAttachShader(shaderRef, vertexShader);
	glAttachShader(shaderRef, fragmentShader);
	glLinkProgram(shaderRef);

	//ERROR HANDLING FOR SHADER PROGRAM
	glGetProgramiv(shaderRef, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderRef, sizeof(infoLog), NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	//DELETE SHADERS AFTER LINKING TO BE TIDY
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

std::string ShaderProgram::readFile(const std::string filePath) {
	try {
		std::ifstream file(filePath);
		std::stringstream buffer;
		buffer << file.rdbuf();
		return buffer.str();
	}
	catch (const std::exception& e) {
		std::cerr << "Error reading file: " << e.what() << std::endl;
		return "";
	}
}
