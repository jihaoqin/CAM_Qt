#ifndef CYLINDER_H
#define CYLINDER_H
#include "GLBinder.h"
#include "glm/glm.hpp"
#include "Mesh.h"


class Cylinder:public GLBinder
{
public:
    Cylinder(glm::vec3 begin_, glm::vec3 end_, double R_);
    virtual void bindGL(QOpenGLContext*) override;
    virtual void draw(std::shared_ptr<GLProgram>) override;
    BoundingBox boundingBox();
private:
    Mesh& generateCyliner(glm::vec3, glm::vec3, float);
    glm::vec3 begin;
    glm::vec3 end;
    double R;
    Mesh m;
};

#endif // CYLINDER_H
