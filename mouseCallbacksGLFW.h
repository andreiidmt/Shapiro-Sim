#pragma once

#include <glad/glad.h>         
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "arcBall.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

inline void mouse_button_signal(GLFWwindow* window, int button, int action, int mods) {
	if (ImGui::GetIO().WantCaptureMouse) return;

	if (button == GLFW_MOUSE_BUTTON_LEFT)
		if (action == GLFW_PRESS){
		arcballCamera::isDragged = true;
		glfwGetCursorPos(window, &arcballCamera::lastX, &arcballCamera::lastY);
	}
		else if (action == GLFW_RELEASE) {
			arcballCamera::isDragged = false;
		}
}

inline void scroll_signal(GLFWwindow* window, double xoffset, double yoffset) {
	if (ImGui::GetIO().WantCaptureMouse) return;
	arcballCamera::zoom(float(yoffset));
}

inline void cursor_moved_signal(GLFWwindow* window, double xposition, double yposition) {
	if (!arcballCamera::isDragged) return;

	float xoffset = (float)(xposition - arcballCamera::lastX);
	float yoffset = (float)(yposition - arcballCamera::lastY);

	arcballCamera::lastX = xposition;
	arcballCamera::lastY = yposition;

	arcballCamera::rotate(xoffset, yoffset);

}