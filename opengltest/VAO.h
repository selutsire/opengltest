#ifndef VAO_H
#define VAO_H

#include <glad/glad.h>
#include <iostream>
#include "VBO.h"

class VAO {
public:
	GLuint ID;

	VAO();
	void linkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);
	void bind();
	void unbind();
	void destruct();
};

#endif