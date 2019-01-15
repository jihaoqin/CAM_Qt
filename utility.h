#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>

namespace utility {
	glm::mat4 createMat(glm::vec3 pos, glm::vec3 zDir, glm::vec3 upDir);
	glm::mat4 setPos(glm::mat4 matrix, glm::vec3 pos);
    glm::mat3 RInMatrix(glm::mat4 matrix);
	double length(glm::vec3 v);
    bool isZero(double);
    void print(glm::vec3 v);
    std::string matLog(glm::mat4 m);
}
