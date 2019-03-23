#ifndef TEECURVE_H
#define TEECURVE_H
#include "Curve.h"
#include "Point.h"

class Tee;
using TeePtr = std::shared_ptr<Tee>;

class TeeCurve:public Curve
{
public:
    TeeCurve(PointPtr, float, float , const char*, TeePtr);
    void updateSelf() override;
    void setWindingAngle(float angle) override;
    void setWindingPara(float angle, float coe) override;
private:
    TeePtr tee;
    PointPtr point;
    float uAng;
    float lambda;
};

#endif // TEECURVE_H
