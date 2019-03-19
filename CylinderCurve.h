#ifndef CYLINDERCURVE_H
#define CYLINDERCURVE_H
#include "Curve.h"
#include "CyCurveAssist.h"

class Point;
using PointPtr = std::shared_ptr<Point>;

class CylinderCurve:public Curve
{
public:
    CylinderCurve(PointPtr, float uAng, float coe, const char* c, Cylinder*);
    void setWindingAngle(float angle) override;
protected:
    void updateSelf() override;
private:
    PointPtr point;
    float uAng;
    float lambda;
    CyCurveAssist assist;
};

#endif // CYLINDERCURVE_H
