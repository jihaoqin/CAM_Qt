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
    BoundingBox boundingBox();
private:
    Mesh generateLeftRing();
    Mesh generateRightRing();
    Mesh generateMainPipe();
    Mesh generateBranchPipe();
    Mesh generateFrontPlane();
    Mesh generateBackPlane();
    Mesh generatePlane(glm::vec3 anchor, std::vector<std::vector<glm::vec3>> edgeVec);
    Mesh generateCylinder(glm::vec3 end_1, glm::vec3 end_2, float r);
    Mesh generateCircle(glm::vec3 anchor, glm::vec3 dir, float r);
    Mesh generateRevolution(glm::vec3 anchor, glm::vec3 dir, std::vector<Vertex> v, float angle);
    std::vector<Mesh> meshVec;
    glm::mat4 modelMat;
    Color color;
    BoundingBox box;
    float pipeR;
    float sideR;
    float lengthMain;
    float lengthBranch;
};

#endif // TEE_H
