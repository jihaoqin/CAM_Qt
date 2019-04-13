#ifndef ENVELOPASSIST_H
#define ENVELOPASSIST_H
#include "NameDeclare.h"
#include "PosDir.h"
class Envelop;
using EnvelopPtr = std::shared_ptr<Envelop>;

class Pipe{
public:
    bool isWorldIn(Pos);
    PosVec interPoints(Pos, Dir);
public:
    float length;
    glm::mat4 T;
    float r;
private:
    bool isLocalIn(Pos);
};
class EnvelopAssist
{
public:
    EnvelopAssist(TeePtr);
    PosVec intersectPoint(Pos , Dir);
private:
    TeePtr tee;
    float m_dis;
    Pipe main;
    Pipe branch;
};

#endif // ENVELOPASSIST_H
