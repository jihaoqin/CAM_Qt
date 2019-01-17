#ifndef TEE_H
#define TEE_H
#include "GLMemory.h"
#include <vector>
#include "vertex.h"
#include "Mesh.h"

class Tee: public GLMemory
{
public:
    Tee();
private:
    Mesh generateLeftRing();
    Mesh generateRightRing();
    Mesh generateCylinder();
    std::vector<Mesh> meshVec;
    float pipeR;
    float sideR;
    float lengthMain;
    float lengthBranch;
};

#endif // TEE_H
