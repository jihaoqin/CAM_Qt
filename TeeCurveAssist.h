#ifndef TEECURVEASSIST_H
#define TEECURVEASSIST_H
#include <memory>
#include "PosDir.h"
#include <QString>
#include <utility>
#include "Edge.h"

class Tee;
using TeePtr = std::shared_ptr<Tee>;
using std::tuple;
using QStringVec = std::vector<QString>;
using Pos = glm::vec3;

class TeeCurveAssist
{
public:
    TeeCurveAssist(TeePtr);
    tuple<PosDirVec, QStringVec, EdgePtrVec, float> genCurve(PosDir p, float coe, QString meshId, float length);
    tuple<PosDirVec, QStringVec, EdgePtrVec, float> genCurve(Pos p, float uAng, float coe, QString meshId, float length);
private:
    TeePtr tee;
};

#endif // TEECURVEASSIST_H
