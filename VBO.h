#pragma once
#include <vector>
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class VBO {
public:
	GLuint id;
	VBO(GLfloat* vertices, GLsizeiptr size);

	VBO(std::vector<float> vertices) : VBO(vertices.data(), vertices.size() * sizeof(float)) {}

	VBO(GLsizeiptr maxSizeBytes) {
		glGenBuffers(1, &id);
		glBindBuffer(GL_ARRAY_BUFFER, id);
		glBufferData(GL_ARRAY_BUFFER, maxSizeBytes, NULL, GL_DYNAMIC_DRAW);
	}

	void update(const std::vector<glm::vec3>& vertices) {
		Bind();
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(glm::vec3), vertices.data());
	}

	void Bind();
	void Unbind();
	void Delete();
};