#ifndef POINT_H
#define POINT_H
#include <glm/glm.hpp>
#include "GLMemory.h"
#include "vertex.h"
#include <vector>

class Point:public GLMemory
{
public:
    Point(glm::vec3 p);
    void bindGL(QOpenGLContext*) override;
    void draw(std::shared_ptr<GLProgram> p) override;
private:
    glm::vec3 pos;
    std::vector<Vertex> vertexVec;
};

#endif // POINT_H
