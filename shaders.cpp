#include "shaders.h"

std::string readShadersFile(const std::string file_name)
{
    std::ifstream fread(file_name);
    if (!fread.is_open())
        throw std::runtime_error("Couldn't open shader source: " + file_name);
    std::string content;
    std::string line;
    while (std::getline(fread, line))
        content += line + "\n";
    return content;
}

Shader::Shader(const std::string vertexFile, const std::string fragmentFile)
{
    std::string vertexCode = readShadersFile(vertexFile);
    std::string fragmentCode = readShadersFile(fragmentFile);

    const char* vertexSource = vertexCode.c_str();
    const char* fragmentSource = fragmentCode.c_str();

    //creates a vertex shader.
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);

    id = glCreateProgram();

    glAttachShader(id, vertexShader);
    glAttachShader(id, fragmentShader);

    glLinkProgram(id);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::activateShader()
{
    glUseProgram(id);
}

void Shader::deleteShader()
{
    glDeleteProgram(id);
}
