#include "Texture.h"

Texture::Texture(const char* imgPath, GLenum textureType, GLuint slot, GLenum pixelType) {
	type = textureType;

	int imgWidth, imgHeight, imgColCh;

	stbi_set_flip_vertically_on_load(true);
	unsigned char* imgData;

	try {
		imgData = stbi_load(imgPath, &imgWidth, &imgHeight, &imgColCh, 0);
	}
	catch (const std::exception& e) {
		std::cerr << "Texture loading error: " << e.what() << std::endl;
	}
	if (imgData == NULL) {
		std::cerr << "Failed to load texture at path: " << imgPath << std::endl;
		return;
	}
	
	GLenum format;
	switch (imgColCh) {
	case 1: format = GL_RED; break;
	case 2: format = GL_RG; break;
	case 3: format = GL_RGB; break;
	case 4: format = GL_RGBA; break;
	default: format = GL_RGBA; break;
	}

	glGenTextures(1, &ID);

	glActiveTexture(GL_TEXTURE0 + slot);
	unit = slot;
	glBindTexture(textureType, ID);

	glTexParameteri(textureType, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(textureType, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (imgData) {
		glTexImage2D(textureType, 0, GL_RGBA, imgWidth, imgHeight, 0, format, pixelType, imgData);
		glGenerateMipmap(textureType);
	}
	else {
		std::cerr << "(texture.cpp)failed to find texture data in texture at path: " << imgPath << std::endl;
	}
	stbi_image_free(imgData);

	glBindTexture(textureType, 0);
}

void Texture::texUnit(ShaderProgram& shaderProgram, const char* uniform, GLuint unit) {
	GLuint texUni = glGetUniformLocation(shaderProgram.ID, uniform);
	if (texUni == -1) {
		std::cerr << "Uniform '" << uniform << "' not found in shader program." << std::endl;
		return;
	}
	shaderProgram.use();
	glUniform1i(texUni, unit);
}

void Texture::bind() {
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(type, ID);
}

void Texture::unbind() {
	glBindTexture(type, 0);
}

void Texture::destruct() {
	glDeleteTextures(1, &ID);
}