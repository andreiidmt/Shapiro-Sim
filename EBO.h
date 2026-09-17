#pragma once
#include <glad/glad.h>
#include <vector>

class EBO {
public:
	GLuint id;
	EBO(GLuint* indices, GLsizeiptr size);

	EBO(std::vector<unsigned int> indices) : EBO(indices.data(), indices.size() * sizeof(unsigned int)) {}

	void Bind();
	void Unbind();
	void Delete();
};