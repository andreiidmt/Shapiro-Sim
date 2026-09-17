#version 330 core
out vec4 FragColor;

in vec3 FragPos;

uniform vec3 u_CamPos;

void main()
{
	vec3 normal = normalize(FragPos);
	vec3 lightDirection = normalize(u_CamPos-FragPos);

	float rim = 1.0-abs(dot(normal,lightDirection));
	rim = pow(rim,2.0);

	vec3 baseDarkColor = vec3(0.02,0.02,0.02);
	vec3 outlineGlow = vec3(1.0,0.745,0.53);

	vec3 finalColor = baseDarkColor + (outlineGlow*rim*1.5);

	FragColor = vec4(finalColor, 1.0f);
}