#ifndef LEFTCYLINDERASSIST_H
#define LEFTCYLINDERASSIST_H
#include <memory>
#include <QString>
#include "PosDir.h"
#include "Band.h"
#include "CPPara.h"
class Tee;
class Cylinder;
using TeePtr = std::shared_ptr<Tee>;
using QStringVec = std::vector<QString>;

class Sol{
public:
    float lam1;
    float lam2;
    float alpha;
};
using SolVec = std::vector<Sol>;

class LeftCylinderAssist
{
public:
    LeftCylinderAssist(TeePtr, QString which);
    std::tuple<PosDirVec, QStringVec> genCurve(EndPtr);
    std::tuple<PosDirVec, QStringVec> genCurve(PosDir);
    std::tuple<PosDirVec, QStringVec> genCircleCurve(EndPtr, EndPtr);
    float endToEndAngle(EndPtr, EndPtr);
    bool isPosIn(Pos);
    bool isPosAtStart(Pos);
    bool isReturn(EndPtr);
    void findCoupleEnd(int ind, std::vector<BandEnd>);
    int findBand(QString, std::vector<BandEnd>);
    CPPara worldToCPPara(PosDir world);
    Dir tanDir(float u, float v, float uCoe, float vCoe);
    CPPara normPara(CPPara&, glm::vec2);
    std::vector<Sol> getSol(CPPara p1, CPPara p2, float absLam);
    std::vector<float> getDus(CPPara p1, float a2, CPPara p3, float r, float absLam);
    PosDir paraToWorld(CPPara);
    EndPtrVec filterDir(EndPtr, EndPtrVec);
    EndPtrVec filterCycle(EndPtr, EndPtrVec, const EndPtrVec);
    EndPtrVec filterInnerFirst(EndPtrVec, const EndPtrVec);
    EndPtr nearEnd(EndPtr, EndPtrVec);
private:
    float lnn(float);
    bool isBad(glm::mat2);
    TeePtr tee;
    glm::mat4 T;
    std::vector<Cylinder*> cylinders;
    float m_length;
    float r;
};

#endif // LEFTCYLINDERASSIST_H
