#version 330 core
out vec4 FragColor;

in vec3 FragPos;

uniform float u_MaxRadius;

void main()
{
	float dist = length(FragPos.xz);
	float fade = 1.0 - smoothstep(0.0, u_MaxRadius, dist);
	
	vec3 baseColor = vec3(0.15, 0.15, 0.15);

	FragColor = vec4(baseColor*fade,1.0f);
}