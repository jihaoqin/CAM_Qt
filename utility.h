#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include "vertex.h"
#include "Mesh.h"
#include <complex>
#include <vector>
#include "PosDir.h"
#include "Band.h"

namespace utility {
const float PI = 3.1415926;
struct Root{
    std::complex<double> x1;
    std::complex<double> x2;
    std::complex<double> x3;
    std::complex<double> x4;
};
Root root4(vector<double> coe);
std::vector<std::complex<double>> findRoot(vector<double> coe);
glm::mat4 createMat(glm::vec3 pos, glm::vec3 zDir, glm::vec3 upDir);
glm::mat4 setPos(glm::mat4& matrix, glm::vec3 pos);
glm::vec3 getPos(glm::mat4 matrix);
glm::vec3 getPos(glm::vec4 homo);
glm::mat4 setXDir(glm::mat4& matrix, glm::vec3 pos);
glm::mat4 setYDir(glm::mat4& matrix, glm::vec3 pos);
glm::mat4 setZDir(glm::mat4& matrix, glm::vec3 pos);
glm::mat3 RInMatrix(glm::mat4 matrix);
glm::vec3 multiply(glm::mat4, glm::vec3, QString);
bool isIn(float x, float x1, float x2);
double length(glm::vec3 v);
double length(glm::vec2 v);
bool isZero(double);
void print(glm::vec3 v);
std::string matLog(glm::mat4 m);
glm::vec3 lineRotatePoint(glm::vec3 anchor, glm::vec3 dir, glm::vec3 p, float alpha);
glm::vec3 lineRotateVector(glm::vec3 line, glm::vec3 vec, float alpha);
Vertex lineRotateVertex(glm::vec3 anchor, glm::vec3 dir, Vertex p, float alpha);
bool isParallel(glm::vec3 vec1, glm::vec3 vec2);
Mesh generateRevolution(glm::vec3 anchor, glm::vec3 dir, std::vector<Vertex> edge, float angle);
vector<unsigned int> filterInterval(vector<glm::vec3>, float h);
vector<float> sameInterval(float x1, float x3, float h);
vector<Pos> filterNum(vector<Pos>, int num);
vector<Pos> filterNum(vector<PosDir>, int num);
bool hasCycle(EndPtrVec);
bool hasEnd(EndPtrVec, QString);
EndPtr getNextEnd(BandEndPtrVec, QString);
EndPtrVec valueCopyEndPtrVec(const EndPtrVec);
EndPtr valueCopyEndPtr(const EndPtr);
EndPtr findEnd(const QString, const EndPtrVec);
EndPtr theOtherEnd(const EndPtr, const EndPtrVec);
}
