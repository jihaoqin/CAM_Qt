#include "TCurve.h"
#include "Point.h"
#include "Tee.h"
#include "RingAssist.h"
#include "CPPara.h"
#include "RingCurveAssist.h"
#include "CylinderAssist.h"
#include "TriEdgePlaneAssist.h"
#include "CyCurveAssist.h"
#include "TriCurveAssist.h"
#include "TCurveAssist.h"

using namespace std;
using PosDirVec = vector<PosDir>;
using EdgePtrVec = vector<EdgePtr>;
TCurve::TCurve(PointPtr p, float a, float coe, const char*c , TeePtr t)
    :Curve(c), point(p), uAng(a), lambda(coe), tee(t)
{
    updateSelf();
}

void TCurve::updateSelf(){
    TCurveAssist assist(tee);
    float pi = asin(1)*2;
    QString id(point->meshId());
    glm::vec3 worldPos = point->getPos();
    auto posDir1 = assist.genCurve(worldPos, uAng, lambda, id);
    std::vector<glm::vec3> points;
    for(auto i = posDir1.first.rbegin(); i!=posDir1.first.rend(); i++){
        points.push_back(i->pos);
    }
    auto posDir2 = assist.genCurve(worldPos, uAng+pi, lambda, id);
    for(auto i:posDir2.first){
        points.push_back(i.pos);
    }
    data(points);
}

void TCurve::setWindingAngle(float angle){
    uAng = angle;
    update();
}

void TCurve::setWindingPara(float angle, float coe){
    uAng = angle;
    lambda = coe;
    update();
}

