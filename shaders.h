#pragma once
#include <glad/glad.h>
#include <string>
#include <fstream>
#include <exception>
#include <sstream>
#include <stdexcept>

std::string readShadersFile(const std::string file_name);

class Shader {
public:
	GLuint id;
	Shader(const std::string vertexFile, const std::string fragmentFile);

	void activateShader();
	void deleteShader();

};