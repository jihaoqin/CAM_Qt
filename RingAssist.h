#ifndef RINGASSIST_H
#define RINGASSIST_H
#include "PosDir.h"
#include "Ring.h"
#include "CPPara.h"

class RingAssist
{
public:
    RingAssist(Ring&);
    glm::vec3 worldDir(CPPara);
    vector<glm::vec3> intersectionPoints(glm::vec3 worldPos, glm::vec3 worldDir);
    vector<double> generateCoe(glm::vec3 worldPos, glm::vec3 worldDir);
    glm::vec3 localNorm(float u, float v);
    vector<float> paraWithLocalPoint(glm::vec3);
    bool isSamePoint(float theta, float alpha, glm::vec3 pos);
    bool inParaSpace(float theta, float alpha);
    PosDir localProject(glm::vec3 pos, glm::vec3 dir);
    PosDir worldProject(glm::vec3 pos, glm::vec3 dir);
    PosDir CPParaToLocal(CPPara p);
    CPPara local3DProjectToPara(glm::vec3 pos, glm::vec3 dir);
    vector<float> local3DProjectToUV(glm::vec3 pos);
    glm::vec3 local3DToWorld(glm::vec3 local, const char*);
    glm::vec3 world3DToLocal(glm::vec3 world, const char*);
    glm::vec3 paraToLocal3D(float theta, float alpha);
    glm::vec3 localTangentDir(float theta, float alpha, double uWeight, double vWeight);
    vector<EdgePtr> getEdges();
    Dir outNorm(Pos);
private:
    double R;
    double r;
    double angle;//弧度制
    glm::mat4 T;
    glm::vec3 anchor;
    glm::vec3 zdir;
    glm::vec3 xdir;
    QString id;
};

#endif // RINGASSIST_H
