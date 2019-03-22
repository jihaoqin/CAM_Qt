#ifndef CYCURVEASSIST_H
#define CYCURVEASSIST_H
#include <glm/glm.hpp>
#include "Cylinder.h"
#include "CylinderAssist.h"
#include "Edge.h"
#include "CPPara.h"
#include "PosDir.h"
#include <utility>
#include <vector>

using std::pair;
using std::vector;
class CyCurveAssist
{
public:
    CyCurveAssist(Cylinder&);
    pair<vector<PosDir>, vector<EdgePtr>> genCurve(glm::vec3 pos, glm::vec3 dir, float coe);
    pair<vector<PosDir>, vector<EdgePtr>> genCurve(glm::vec3 pos, float uAng, float coe);
private:
    pair<vector<CPPara>, vector<EdgePtr>> genCurve(CPPara p, float coe);
    pair<vector<CPPara>, EdgePtr> rungeKutta(CPPara p, QString s, float coe);
    CPPara evalNext(float s, CPPara p, float coe, float h);
    vector<float> ringDiff(float s, vector<float> y0, float coe);
    float lambda;
    glm::vec3 anchor;
    glm::vec3 zDir;
    glm::vec3 xDir;
    float length;
    float angle;
    float R;
    vector<EdgePtr> edges;
    CylinderAssist assist;

};

#endif // CYCURVEASSIST_H
