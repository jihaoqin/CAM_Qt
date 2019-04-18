#ifndef ENVELOPASSIST_H
#define ENVELOPASSIST_H
#include "NameDeclare.h"
#include "PosDir.h"
#include "HalfPoint.h"
#include <QString>
#include "CPPara.h"
class Envelop;
using EnvelopPtr = std::shared_ptr<Envelop>;

class Pipe{
public:
    bool isWorldIn(Pos);
    PosVec interPoints(Pos, Dir);
    PosVec genInternalSuper(PosVec);
    bool isCross(Pos, Pos);
public:
    Pos paraToLocal(SPara);
    Pos paraToWorld(SPara);
    float length;
    glm::mat4 T;
    float r;
private:
    Pos invTrans(glm::vec3 , QString);
    SPara localPara(Pos);
    bool isLocalIn(Pos);
};
class EnvelopAssist
{
public:
    EnvelopAssist(TeePtr);
    SuperPosVec intersectPoint(Pos , Dir);
    bool isCross(Pos, Pos);
    SuperPosVec genInsertedSuper(SuperPosVec);
    SuperPosVec genInsertedInOne(SuperPosVec);
private:
    TeePtr tee;
    float m_dis;
    Pipe main;
    Pipe branch;
    Pipe mainPipe;
    Pipe branchPipe;
};

#endif // ENVELOPASSIST_H
