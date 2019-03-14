#include "RingCurve.h"
#include "Point.h"
#include <memory>
#include "RingCurveAssist.h"

RingCurve::RingCurve(PointPtr p, float angle, float coe, const char* c, Ring* ring)
    :Curve(c), assist(*ring)
{
    assert(p);
    QString id(p->getId());
    if(!id.contains("ring")){
        assert(0);
    }
    point = p;
    uAng = angle;
    lambda = coe;
    glm::vec3 pos = p->getPos();
    auto posDirs = assist.genCurve(pos, uAng, lambda);
    std::vector<glm::vec3> points;
    for(auto i:posDirs){
        points.push_back(i.pos);
    }
    data(points);
    写到这里了，改写将RingCurve加入到树中的代码了
}
