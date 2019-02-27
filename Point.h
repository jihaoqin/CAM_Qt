#ifndef POINT_H
#define POINT_H
#include <glm/glm.hpp>
#include "DataObject.h"
#include "vertex.h"
#include <vector>
#include "Color.h"
#include "boundingbox.h"

class Point:public DataObject
{
public:
    Point(glm::vec3 p, const char* c);
    void bindGL(QOpenGLContext*) override;
    virtual void draw(std::shared_ptr<GLProgram>) override;
private:
    void updateBoundingBox();
    glm::vec3 pos;
    std::vector<Vertex> vertexVec;
    std::vector<unsigned int> indexVec;
    QOpenGLFunctions_4_3_Core *core;
    unsigned int VAO,VBO,EBO;
    Color color;
    BoundingBox box;
};

#endif // POINT_H
