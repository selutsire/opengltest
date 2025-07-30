#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <iostream>
#include <stb/stb_image.h>

#include "ShaderProgram.h"

class Texture {
public:
	GLuint ID;
	GLenum type;
	GLuint unit;

	Texture(const char* imgPath, GLenum textureType, GLuint slot, GLenum pixelType);
	void texUnit(ShaderProgram& shader, const char* uniform, GLuint unit);
	void bind();
	void unbind();
	void destruct();
};

#endif