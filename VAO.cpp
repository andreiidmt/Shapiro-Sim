#include "VAO.h"

VAO::VAO()
{
	glGenVertexArrays(1, &id);
}

void VAO::linkVBO(VBO v, GLuint layout)
{
	v.Bind();
	glVertexAttribPointer(layout, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(layout);
	v.Unbind();
}

void VAO::Bind()
{
	glBindVertexArray(id);
}

void VAO::Unbind()
{
	glBindVertexArray(0);
}

void VAO::Delete()
{
	glDeleteVertexArrays(1, &id);
}
