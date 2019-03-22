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

class TeeCurveAssist
{
public:
    TeeCurveAssist(TeePtr);
    tuple<PosDirVec, QStringVec, EdgePtrVec> genCurve(PosDir p, float coe, QString meshId, float length);
private:
    TeePtr tee;
};

#endif // TEECURVEASSIST_H
