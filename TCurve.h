#ifndef TCURVE_H
#define TCURVE_H
#include "PosDir.h"
#include "Curve.h"
#include <vector>
#include <memory>
class Point;
class Tee;
using PointPtr = std::shared_ptr<Point>;
using TeePtr = std::shared_ptr<Tee>;


class TCurve:public Curve
{
public:
    TCurve(PointPtr, float, float , const char*, TeePtr);
private:
    void genCurve(PosDir pd, QString meshId);
    PointPtr designP;
    TeePtr tee;
    float uAng;
    float lambda;
    std::vector<PosDir> points;
};

#endif // CURVE_H
