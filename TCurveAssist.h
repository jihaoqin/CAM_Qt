#ifndef TCURVEASSIST_H
#define TCURVEASSIST_H
#include "PosDir.h"
#include "Edge.h"
#include <utility>
#include <map>
#include "Point.h"

class Tee;
using TeePtr = std::shared_ptr<Tee>;
using Pos = glm::vec3;
using Dir = glm::vec3;
using QStringVec = std::vector<QString>;
class TCurveAssist
{
public:
    TCurveAssist(TeePtr t);
    std::tuple<PosDirVec, QStringVec, EdgePtrVec> genCurve(Pos pd, float uAng, float coe, QString meshId);
    std::tuple<PosDirVec, QStringVec, EdgePtrVec> genCurve(PosDir pd, float coe, QString meshId);
    PosDir genCurve(PosDir pd, QString meshId, float length);
    bool sameEdge(EdgePtr, EdgePtr);
private:
    TeePtr tee;
};

#endif // TCURVEASSIST_H
