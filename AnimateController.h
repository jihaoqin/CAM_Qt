#ifndef ANIMATECONTROLLER_H
#define ANIMATECONTROLLER_H
#include "NameDeclare.h"

class AnimateController
{
public:
    AnimateController(Controller*);
    void initBandPtrs();
    void initIndexPairVecs();
private:
    Controller* ctrl;
    BandPtrVec bandPtrs;
    std::vector<GLIndexPairVec> indexPairVecs;
    float m_distance;
};

#endif // ANIMATECONTROLLER_H
