#ifndef TRICURVEASSIST_H
#define TRICURVEASSIST_H
#include "Curve.h"
#include "TriEdgePlane.h"
#include "TriEdgePlaneAssist.h"
#include <utility>

class TriCurveAssist
{
public:
    TriCurveAssist(TriEdgePlane&);
    std::vector<PosDir> genCurve(glm::vec3 pos, glm::vec3 dir, float coe);
    std::vector<PosDir> genCurve(glm::vec3 pos, float uAng, float coe);
private:
    std::pair<vector<CPPara>, vector<EdgePtr>> genCurve(CPPara);
    float R;
    glm::vec3 uDir;
    glm::vec3 anchor;
    glm::vec3 norm;
    std::vector<EdgePtr> edges;
    TriEdgePlaneAssist assist;
};

#endif // TRICURVEASSIST_H
