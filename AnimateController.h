#ifndef ANIMATECONTROLLER_H
#define ANIMATECONTROLLER_H
#include "NameDeclare.h"
class AnimateController
{
public:
    AnimateController(Controller*);
    void showNext();
    void calculation();
    int getPercent();
    void resetBand();
    void setPercent(int);
private:
    void initBandPtrs();
    void initIndexPairVecs();
    void initHangingBand();
    Controller* ctrl;
    BandPtrVec bandPtrs;
    std::vector<GLIndexPairVec> indexPairVecs;
    int pairTotal;
    int windedTotal;
    int bandInd;
    int showInd;
    float m_distance;
};

#endif // ANIMATECONTROLLER_H
