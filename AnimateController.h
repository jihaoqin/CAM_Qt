#ifndef ANIMATECONTROLLER_H
#define ANIMATECONTROLLER_H
#include "NameDeclare.h"
#include "MoveData.h"
#include "PosDir.h"
class AnimateController
{
public:
    AnimateController(Controller*);
    void showNext();
    void calculation();
    int getPercent();
    void resetBand();
    void setPercent(int);
    void resetAnimateT();
    std::pair<int, int> getInsertInd(int);
    bool hasData();
    void hideBandSet();
    Pos currentPos();
private:
    void initBandPtrs();
    void initIndexPairVecs();
    void initHangingBand();
    void solveCollision();
    void initBandPos();
    void setRotxs();
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
