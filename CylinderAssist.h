#ifndef CYLINDERASSIST_H
#define CYLINDERASSIST_H
#include <glm/glm.hpp>
#include "Cylinder.h"
#include "CPPara.h"
#include "PosDir.h"

class CylinderAssist
{
public:
    CylinderAssist(Cylinder&);
    vector<float> local3DProjectToUV(glm::vec3 pos);
    vector<float> local3DToUV(glm::vec3 pos);
    glm::vec3 local3DToWorld(glm::vec3 local, const char*);
    glm::vec3 world3DToLocal(glm::vec3 world, const char*);
    CPPara local3DProjectToPara(glm::vec3 pos, glm::vec3 dir);
    glm::vec3 localDir(float u, float v, float uWeight, float vWeight);
    PosDir paraToWorld(CPPara);
    vector<EdgePtr> getEdges();
private:
    glm::vec3 anchor;
    glm::vec3 zDir;
    glm::vec3 xDir;
    glm::mat4 T;
    QString id;
    float length;
    float angle;
    float R;
};

#endif // CYLINDERASSIST_H
