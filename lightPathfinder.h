#pragma once
#include <glad/glad.h>         
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Objects.h"
#include "Computations.h"

#include <limits>
#include <vector>

std::vector<glm::vec3> generateBentLight(glm::vec3 start, glm::vec3 destination, float massScale, bool isBlackHole,
	float scaledRadius, int steps = 200);

std::vector<glm::vec3> generateUnperturbedLight(glm::vec3 start, glm::vec3 destination, float dashLength = 0.4f, float gapLength = 0.25f);
