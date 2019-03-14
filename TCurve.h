#ifndef TCURVE_H
#define TCURVE_H
#include "PosDir.h"
#include <vector>
#include <memory>
class Point;
class Tee;
using PointPtr = std::shared_ptr<Point>;
using TeePtr = std::shared_ptr<Tee>;


class TCurve
{
public:
    TCurve(PointPtr, float, TeePtr);
private:
    PointPtr designP;
    TeePtr tee;
    float uAng;
    std::vector<PosDir> points;
};

#endif // CURVE_H
