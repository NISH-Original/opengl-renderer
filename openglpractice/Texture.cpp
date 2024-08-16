#include "Texture.h"

Texture::Texture(const char* image, const char* texType, GLuint slot)
{
	// assign texture type
	type = texType;

	// stores the width, height, and color channels
	int widthImg, heightImg, numColCh;
	// flips the image so it appears right side up
	stbi_set_flip_vertically_on_load(true);
	// reads image from file and stores in bytes
	unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, &numColCh, 0);

	glGenTextures(1, &ID);
	// assign texture to texture unit
	glActiveTexture(GL_TEXTURE0 + slot);
	unit = slot;
	glBindTexture(GL_TEXTURE_2D, ID);

	// configure type of algorithm that is used to
	// minimise and magnify the texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// configures the way the texture repeats (or if it does at all)
	// S, T, R, correspond to x, y, and z axes
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// extra lines in case you choose to use GL_CLAMP_TO_BORDER
	// float flatColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
	// glTextParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, flatColor);

	if (numColCh == 4)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
	else if (numColCh == 3)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, GL_RGB, GL_UNSIGNED_BYTE, bytes);
	else if (numColCh == 1)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, GL_RED, GL_UNSIGNED_BYTE, bytes);
	else
		throw std::invalid_argument("Automatic texture type recognition failed!");

	glGenerateMipmap(GL_TEXTURE_2D);

	// deletes image data as it is already in the opengl texture object
	stbi_image_free(bytes);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::texUnit(Shader& shader, const char* uniform, GLuint unit)
{
	GLuint texUni = glGetUniformLocation(shader.ID, uniform);
	shader.Activate();
	glUniform1i(texUni, unit);
}

void Texture::Bind()
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Delete()
{
	glDeleteTextures(1, &ID);
}