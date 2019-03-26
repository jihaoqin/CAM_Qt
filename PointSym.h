#ifndef POINTSYM_H
#define POINTSYM_H
#include "Point.h"
#include "PosDir.h"

class Tee;
using TeePtr = std::shared_ptr<Tee>;
using TeeWptr = std::weak_ptr<Tee>;


class PointSym:public Point
{
public:
    PointSym(PointPtr, QString, QString, TeePtr);
    virtual Pos getPos() override;
private:
    void updateSelf() override;
    PointWptr point;
    TeeWptr tee;
    QString symFlag;
};

using PointSymPtr = std::shared_ptr<PointSym>;

#endif // POINTSYM_H
