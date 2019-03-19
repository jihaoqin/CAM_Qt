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
using PointPtr = std::shared_ptr<Point>;
using TeePtr = std::shared_ptr<Tee>;
using std::vector;

class TCurve:public Curve
{
public:
    TCurve(PointPtr, float, float , const char*, TeePtr);
    void updateSelf() override;
private:
    std::pair<vector<PosDir>, vector<EdgePtr>> genCurve(PosDir pd, float coe, QString meshId);
    PointPtr point;
    TeePtr tee;
    float uAng;
    float lambda;
    std::vector<PosDir> points;
};

#endif // CURVE_H
