#ifndef TEECURVE_H
#define TEECURVE_H
#include "Curve.h"
#include "Point.h"

class Tee;
using TeePtr = std::shared_ptr<Tee>;

class TeeCurve:public Curve
{
public:
    TeeCurve(PointPtr p, float a, float coe, const char* c, TeePtr t);
    TeeCurve(PointPtr p, Dir d, float coe, const char* c, TeePtr t);
    void updateSelf() override;
    void setWindingAngle(float angle) override;
    void setWindingPara(float angle, float coe) override;
private:
    TeePtr tee;
    PointPtr point;
    float uAng;
    float lambda;
};
using TeeCurvePtr = std::shared_ptr<TeeCurve>;

#endif // TEECURVE_H
