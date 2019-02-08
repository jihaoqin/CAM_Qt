#ifndef CYLINDER_H
#define CYLINDER_H
#include "GLMemory.h"
#include "glm/glm.hpp"


class Cylinder:public GLMemory
{
public:
    Cylinder();
private:
    glm::vec3 begin;
    glm::vec3 end;
    double R;
};

#endif // CYLINDER_H
