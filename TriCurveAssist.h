#ifndef TRICURVEASSIST_H
#define TRICURVEASSIST_H
#include "Curve.h"
#include "Edge.h"
#include "TriEdgePlane.h"
#include "TriEdgePlaneAssist.h"
#include <utility>
using std::vector;
using std::tuple;
class TriCurveAssist
{
public:
    TriCurveAssist(TriEdgePlane&);
    tuple<PosDirVec, EdgePtrVec, float> genCurve(glm::vec3 pos, glm::vec3 dir, float coe, float length = 1e6);
    tuple<PosDirVec, EdgePtrVec, float> genCurve(glm::vec3 pos, float uAng, float coe, float length = 1e6);
    float length3D(CPPara, CPPara);
private:
    tuple<CPParaVec, EdgePtrVec, float> genCurve(CPPara, float, float);
    float R;
    glm::vec3 uDir;
    glm::vec3 anchor;
    glm::vec3 norm;
    std::vector<EdgePtr> edges;
    TriEdgePlaneAssist assist;
};

#endif // TRICURVEASSIST_H
