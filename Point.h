#ifndef POINT_H
#define POINT_H
#include <glm/glm.hpp>
#include "GLBinder.h"

class Point:public GLBinder
{
public:
    Point(glm::vec3 p);
    void bindGL(QOpenGLContext*) override;
    void draw(std::shared_ptr<GLProgram> p) override;
private:
    glm::vec3 pos;
};

#endif // POINT_H
