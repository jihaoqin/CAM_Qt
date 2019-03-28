#include "RingCurve.h"
#include "Point.h"
#include <memory>
#include "RingCurveAssist.h"

RingCurve::RingCurve(PointPtr p, float angle, float coe, const char* c, Ring* ring)
    :Curve(c), point(p), uAng(angle), lambda(coe), assist(*ring)
{
    type = "RingCurve";
    updateSelf();
}

void RingCurve::updateSelf(){
    assert(point);
    QString id(point->meshId());
    if(!id.contains("ring")){
        assert(0);
    }
    glm::vec3 pos = point->getPos();
    auto posDirs = assist.genCurve(pos, uAng, lambda);
    std::vector<glm::vec3> points;
    for(auto i:std::get<0>(posDirs)){
        points.push_back(i.pos);
    }
    data(points);
}

void RingCurve::setWindingAngle(float angle){
    uAng = angle;
    update();
}
