#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace utility {
	glm::mat4 createMat(glm::vec3 pos, glm::vec3 zDir, glm::vec3 upDir);
	glm::mat4 setPos(glm::mat4 matrix, glm::vec3 pos);
	double length(glm::vec3 v);
}