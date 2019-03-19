#ifndef TRIEDGEPLANECURVE_H
#define TRIEDGEPLANECURVE_H
#include "TriEdgePlaneAssist.h"
#include "TriCurveAssist.h"
#include "Curve.h"

class Point;
using PointPtr = std::shared_ptr<Point>;

class TriEdgePlaneCurve:public Curve
{
public:
    TriEdgePlaneCurve(PointPtr, float uAng_, float coe, const char* , TriEdgePlane*);
    void setWindingAngle(float angle) override;
protected:
    void updateSelf() override;
private:
    PointPtr point;
    float uAng;
    float lambda;
    TriCurveAssist assist;
};

#endif // TRIEDGEPLANECURVE_H
