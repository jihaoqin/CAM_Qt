#ifndef TEE_H
#define TEE_H
#include "GLBinder.h"
#include <vector>
#include "vertex.h"
#include "Mesh.h"
#include "Color.h"

class Tee: public GLBinder
{
public:
    Tee();
    virtual void bindGL(QOpenGLContext*) override;
    virtual void draw(std::shared_ptr<GLProgram>) override;
private:
    Mesh generateLeftRing();
    Mesh generateRightRing();
    Mesh generateCylinder();
    std::vector<Mesh> meshVec;
    glm::mat4 modelMat;
    Color color;
    float pipeR;
    float sideR;
    float lengthMain;
    float lengthBranch;
};

#endif // TEE_H
