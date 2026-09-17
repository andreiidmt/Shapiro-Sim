#pragma once
#include <glad/glad.h>
#include "VBO.h"
class VAO {
public:
	GLuint id;
	VAO();

	void linkVBO(VBO v, GLuint layout);
	void Bind();
	void Unbind();
	void Delete();
};