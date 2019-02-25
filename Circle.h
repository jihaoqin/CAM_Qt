#ifndef CIRCLE_H
#define CIRCLE_H

#include "GLBinder.h"
#include "Mesh.h"

class Circle:public GLBinder
{
public:
    Circle(glm::vec3 center_, glm::vec3 zdir_, float r_);
private:
    glm::vec3 center;
    glm::vec3 zdir;
    float r;
    Mesh m;
};

#endif // CIRCLE_H
