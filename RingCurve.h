#ifndef RINGCURVE_H
#define RINGCURVE_H
#include "Curve.h"
#include "RingCurveAssist.h"

class Point;
class Ring;
using PointPtr = std::shared_ptr<Point>;
class RingCurve:public Curve
{
public:
    RingCurve(PointPtr, float angle, float coe, const char* , Ring*);
private:
    PointPtr point;
    float uAng;
    float lambda;
    RingCurveAssist assist;
};

#endif // RINGCURVE_H
