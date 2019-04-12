#ifndef ENVELOPASSIST_H
#define ENVELOPASSIST_H
#include "NameDeclare.h"
#include "PosDir.h"
class Envelop;
using EnvelopPtr = std::shared_ptr<Envelop>;
class EnvelopAssist
{
public:
    EnvelopAssist(TeePtr, EnvelopPtr);
    PosVec intersectPoint(Pos , Dir);
private:
    TeePtr tee;
    float m_dis;
    glm::mat4 m_T1;
    glm::mat4 m_T2;
};

#endif // ENVELOPASSIST_H
