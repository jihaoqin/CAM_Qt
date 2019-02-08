#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include "vertex.h"
#include "Mesh.h"

namespace utility {
    const float PI = 3.1415926;
	glm::mat4 createMat(glm::vec3 pos, glm::vec3 zDir, glm::vec3 upDir);
    glm::mat4 setPos(glm::mat4& matrix, glm::vec3 pos);
    glm::mat4 setXDir(glm::mat4& matrix, glm::vec3 pos);
    glm::mat4 setYDir(glm::mat4& matrix, glm::vec3 pos);
    glm::mat4 setZDir(glm::mat4& matrix, glm::vec3 pos);
    glm::mat3 RInMatrix(glm::mat4 matrix);
	double length(glm::vec3 v);
    bool isZero(double);
    void print(glm::vec3 v);
    std::string matLog(glm::mat4 m);
    glm::vec3 lineRotatePoint(glm::vec3 anchor, glm::vec3 dir, glm::vec3 p, float alpha);
    glm::vec3 lineRotateVector(glm::vec3 line, glm::vec3 vec, float alpha);
    Vertex lineRotateVertex(glm::vec3 anchor, glm::vec3 dir, Vertex p, float alpha);
    bool isParallel(glm::vec3 vec1, glm::vec3 vec2);
    Mesh generateRevolution(glm::vec3 anchor, glm::vec3 dir, std::vector<Vertex> edge, float angle);
}
