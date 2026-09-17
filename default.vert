#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 FragPos;

uniform mat4 u_MVP;
uniform mat4 u_Model;

void main()
{
	FragPos = vec3(u_Model*vec4(aPos,1.0));
	gl_Position = u_MVP * vec4(aPos, 1.0);
}
