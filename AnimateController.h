#ifndef ANIMATECONTROLLER_H
#define ANIMATECONTROLLER_H
#include "NameDeclare.h"
class Axis3Data{
public:
    float theta;//主轴回转
    float x;//横向平移
    float z;//纵向平移
};

class Axis4Data{
public:
    float theta;//主轴回转
    float x;//横向平移
    float z;//纵向平移
    float flip;//翻滚坐标
};
class Axis5Data{
public:
    float theta;//主轴回转
    float x;//横向平移
    float z;//纵向平移
    float flip;//翻滚坐标
    float yaw;//偏摆坐标
};
using Axis3DataVec = std::vector<Axis3Data>;
using Axis4DataVec = std::vector<Axis4Data>;
using Axis5DataVec = std::vector<Axis5Data>;
class AnimateController
{
public:
    AnimateController(Controller*);
    void showNext();
    void calculation();
    int getPercent();
    void resetBand();
    void setPercent(int);
    std::pair<int, int> getInsertInd(int);
    bool hasData();
    void hideBandSet();
    Axis3DataVec axis3Data();
    Axis4DataVec axis4Data();
    Axis5DataVec axis5Data();
private:
    void initBandPtrs();
    void initIndexPairVecs();
    void initHangingBand();
    void solveCollision();
    void initBandPos();
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
