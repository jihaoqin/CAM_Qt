#ifndef RING_H
#define RING_H
#include "Mesh.h"
#include "glm/glm.hpp"
#include <vector>

class Ring:public GLBinder
{
public:
    Ring(double R_, double r_, double angle_ , glm::vec3 anchor_, glm::vec3 zdir_, glm::vec3 xdir_);
    virtual void bindGL(QOpenGLContext *) override;
    virtual void draw(std::shared_ptr<GLProgram>) override;
    BoundingBox boundingBox();
private:
    double R;
    double r;
    double angle;//弧度制
    glm::vec3 anchor;
    glm::vec3 zdir;
    glm::vec3 xdir;
    Mesh m;
};

#endif // RING_H
