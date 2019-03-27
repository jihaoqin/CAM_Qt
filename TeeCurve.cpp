#include "TeeCurve.h"
#include "TeeCurveAssist.h"
#include "CylinderAssist.h"
#include "RingAssist.h"
#include "TriEdgePlaneAssist.h"
#include "Tee.h"
using namespace std;

TeeCurve::TeeCurve(PointPtr p, float a, float coe, const char* c, TeePtr t)
    :Curve(c), tee(t), point(p), lambda(coe), uAng(a)
{
    father.push_back(weak_ptr<Point>(point));
    updateSelf();
}
TeeCurve::TeeCurve(PointPtr p, Dir d, float coe, const char* c, TeePtr t)
    :Curve(c), tee(t), point(p), lambda(coe)
{
    QString meshId = p->meshId();
    if(meshId.contains("ring")){
        RingAssist rAss(*(tee->getRing(meshId)));
        Pos localPos = rAss.world3DToLocal(p->getPos(),"pos");
        Dir localDir = rAss.world3DToLocal(d, "dir");
        CPPara para = rAss.local3DProjectToPara(localPos, localDir);
        uAng = para.uAng;
    }
    else if(meshId.contains("cylinder")){
        CylinderAssist cyAss(*(tee->getCylinder(meshId)));
        Pos localPos = cyAss.world3DToLocal(p->getPos(), "pos");
        Dir localDir = cyAss.world3DToLocal(d, "dir");
        CPPara para = cyAss.local3DProjectToPara(localPos, localDir);
        uAng = para.uAng;
    }
    else if(meshId.contains("plane")){
        TriEdgePlaneAssist trAss(*(tee->getTriPlane(meshId)));
        Pos localPos = trAss.world3DToLocal(p->getPos(), "pos");
        Dir localDir = trAss.world3DToLocal(d, "dir");
        CPPara para = trAss.local3DProjectToPara(localPos, localDir);
        uAng = para.uAng;
    }
    else{
        assert(0);
    }
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
