#ifndef TRIEDGEPLANEASSIST_H
#define TRIEDGEPLANEASSIST_H
#include "TriEdgePlane.h"
#include "CPPara.h"
#include "PosDir.h"
#include <glm/glm.hpp>

class TriEdgePlaneAssist
{
public:
    TriEdgePlaneAssist(TriEdgePlane&);
    vector<glm::vec3> intersectionPoitns(glm::vec3 worldPos, glm::vec3 worldDir);
    glm::vec3 worldDir(CPPara);
    glm::vec3 localNorm(float u, float v);
    vector<float> paraWithLocalPoint(glm::vec3);
    bool isSamePoint(float u, float v, glm::vec3 pos);
    bool inParaSpace(float u, float v);
    PosDir localProject(glm::vec3 pos, glm::vec3 dir);
    vector<float> local3DToUV(glm::vec3 pos);
    PosDir worldProject(glm::vec3 pos, glm::vec3 dir);
    PosDir CPParaToLocal(CPPara p);
    CPPara local3DProjectToPara(glm::vec3 pos, glm::vec3 dir);
    vector<float> local3DProjectToUV(glm::vec3 pos);
    glm::vec3 local3DToWorld(glm::vec3 local, const char*);
    glm::vec3 world3DToLocal(glm::vec3 world, const char*);
    glm::vec3 paraToLocal3D(float u, float v);
    glm::vec3 localTangentDir(float u, float v, double uWeight, double vWeight);
    vector<EdgePtr> getEdges();

private:
    float R;
    glm::vec3 uDir;
    glm::vec3 anchor;
    glm::vec3 norm;
    glm::mat4 T;
    QString id;
};

#endif // TRIEDGEPLANEASSIST_H
