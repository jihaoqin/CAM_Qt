#include "TriEdgePlaneCurve.h"
#include "Point.h"

TriEdgePlaneCurve::TriEdgePlaneCurve(PointPtr p, float uAng_, float coe, const char* c, TriEdgePlane* tri)
    :Curve(c), point(p), uAng(uAng_), lambda(coe), assist(*tri)
{
    updateSelf();
}

void TriEdgePlaneCurve::updateSelf(){
    assert(point);
    QString id(point->meshId());
    if(!id.contains("plane")){
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

void TriEdgePlaneCurve::setWindingAngle(float angle){
    uAng = angle;
    update();
}
