#include "CylinderCurve.h"
#include "Point.h"

CylinderCurve::CylinderCurve(PointPtr p, float uAng_, float coe, const char* c, Cylinder* cy)
    :Curve(c), point(p), uAng(uAng_), lambda(coe), assist(*cy)
{
    updateSelf();
}

void CylinderCurve::updateSelf(){
    assert(point);
    QString id(point->meshId());
    if(!id.contains("cylinder")){
        assert(0);
    }
    glm::vec3 pos = point->getPos();
    //pos = glm::vec3{1.09647, -7.01166, 7.130417};
    auto posDirs = assist.genCurve(pos, uAng, lambda);
    std::vector<glm::vec3> points;
    for(auto i:std::get<0>(posDirs)){
        points.push_back(i.pos);
    }
    data(points);
}


void CylinderCurve::setWindingAngle(float angle){
    uAng = angle;
    update();
}
