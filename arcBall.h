#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class arcballCamera {
public:
	inline static float radius = 15.0f;
	inline static float yaw = -90.0f;
	inline static float pitch = 20.0f;
	inline static glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f);

	inline static bool isDragged = false;
	inline static double lastX = 0.0f;
	inline static double lastY = 0.0f;


	static void rotate(float xoffset, float yoffset, float sensitivity = 0.2f) {
		yaw += xoffset * sensitivity;
		pitch += yoffset * sensitivity;

		//avoids flipping camera overhead
		if (pitch > 89.0f) pitch = 89.0f;
		if (pitch < -89.0f) pitch = -89.0f;
	}

	static void zoom(float yoffset, float sensitivity = 1.0f) {
		radius -= yoffset * sensitivity;
		if (radius < 2.0f) radius = 2.0f;
		if (radius > 80.0f) radius = 80.0f;
	}

	static glm::vec3 getPosition() {
		float radYaw = glm::radians(yaw);
		float radPitch = glm::radians(pitch);

		float x = target.x + radius * cos(radPitch) * cos(radYaw);
		float y = target.y + radius * sin(radPitch);
		float z = target.z + radius * cos(radPitch) * sin(radYaw);

		return glm::vec3(x, y, z);
	}

	static glm::mat4 getViewMatrix() {
		return glm::lookAt(getPosition(), target, glm::vec3(0.0f, 1.0f, 0.0f));
	}
};