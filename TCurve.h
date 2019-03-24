#ifndef TCURVE_H
#define TCURVE_H
#include "PosDir.h"
#include "Curve.h"
#include "Edge.h"
#include <vector>
#include <memory>
#include <utility>
class Point;
class Tee;
class TCurve;
using PointPtr = std::shared_ptr<Point>;
using TeePtr = std::shared_ptr<Tee>;
using std::vector;
using TCurvePtr = std::shared_ptr<TCurve>;

class TCurve:public Curve
{
public:
    TCurve(PointPtr, float, float , const char*, TeePtr);
    void updateSelf() override;
    void setWindingAngle(float angle) override;
    void setWindingPara(float angle, float coe) override;
    float slip();
    float windAngle();
private:
    PointPtr point;
    TeePtr tee;
    float uAng;
    float lambda;
    std::vector<PosDir> points;
};

#endif // CURVE_H
