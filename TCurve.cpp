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
    type = "TCurve";
    father.push_back(weak_ptr<Point>(point));
    updateSelf();
}

void TCurve::updateSelf(){
    TCurveAssist assist(tee);
    float pi = asin(1)*2;
    QString id(point->meshId());
    glm::vec3 worldPos = point->getPos();
    auto posDir1 = assist.genCurve(worldPos, uAng, lambda, id);
    PosDirVec pd1 = get<0>(posDir1);
    std::vector<glm::vec3> points;
    for(auto i = pd1.rbegin(); i!=pd1.rend(); i++){
        points.push_back(i->pos);
    }
    auto posDir2 = assist.genCurve(worldPos, uAng+pi, lambda, id);
    PosDirVec pd2 = get<0>(posDir2);
    for(auto i:pd2){
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


float TCurve::slip(){
    return lambda;
}

float TCurve::windAngle(){
    return uAng;
}

void TCurve::serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const
{
    writer.StartObject();
    writer.String("type");
    writer.String("TCurve");
    writer.String("uAng");
    writer.Double(uAng);
    writer.String("slip");
    writer.Double(lambda);
    writer.String("point");
    point->serialize(writer);
    writer.EndObject();
}
