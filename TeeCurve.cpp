#include "TeeCurve.h"
#include "TeeCurveAssist.h"
using namespace std;

TeeCurve::TeeCurve(PointPtr p, float a, float coe, const char* c, TeePtr t)
    :Curve(c), tee(t), point(p), lambda(coe), uAng(a)
{
    updateSelf();
}

void TeeCurve::updateSelf(){
    TeeCurveAssist assist(tee);
    float pi = asin(1)*2;
    QString id(point->meshId());
    glm::vec3 worldPos = point->getPos();
    auto posDir1 = assist.genCurve(worldPos, uAng, lambda, id, -15);
    vector<Pos> points;
    for(auto i:get<0>(posDir1)){
        points.push_back(i.pos);
    }
    data(points);
}

void TeeCurve::setWindingAngle(float angle){
    uAng = angle;
    update();
}
void TeeCurve::setWindingPara(float angle, float coe){
    uAng = angle;
    lambda = coe;
    update();
}
