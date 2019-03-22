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

using namespace std;
using PosDirVec = vector<PosDir>;
using EdgePtrVec = vector<EdgePtr>;
TCurve::TCurve(PointPtr p, float a, float coe, const char*c , TeePtr t)
    :Curve(c), point(p), uAng(a), lambda(coe), tee(t)
{
    topo = tee->edgeTopo();
    updateSelf();
}

std::pair<vector<PosDir>, vector<EdgePtr>> TCurve::genCurve(PosDir pd, float coe, QString meshId, QString lastMesh){
    std::pair<vector<PosDir>,vector<EdgePtr>> middle;
    std::pair<vector<PosDir>,vector<EdgePtr>> back;
    if(meshId.contains("ring")){
        RingCurveAssist assist(*(tee->getRing(meshId)));
        middle = assist.genCurve(pd.pos, pd.dir, coe);
    }
    else if(meshId.contains("cylinder")){
        CyCurveAssist assist(*(tee->getCylinder(meshId)));
        middle = assist.genCurve(pd.pos, pd.dir, coe);
    }
    else if(meshId.contains("plane")){
        TriCurveAssist assist(*(tee->getTriPlane(meshId)));
        middle = assist.genCurve(pd.pos, pd.dir, coe);
    }
    else{
        assert(0);
    }
    PosDir side2 = middle.first.back();
    EdgePtr edge2 = middle.second.back();

    QString val2 = tee->topoValue(edge2->Id());
    if(!val2.isEmpty()){
        int i = val2.indexOf("_");
        QString nextMesh = val2.left(i);
        back = genCurve(side2, coe, nextMesh, meshId);
    }
    else{
        back = pair<vector<PosDir>, vector<EdgePtr>>(PosDirVec{side2, side2}, EdgePtrVec{edge2, edge2});
    }
    vector<PosDir> pds;
    vector<EdgePtr> edges;

    for(auto i:middle.first){
        pds.push_back(i);
    }
    edges.push_back(middle.second.front());

    for(auto i:back.first){
        pds.push_back(i);
    }
    edges.push_back(back.second.back());

    return std::pair<vector<PosDir>, vector<EdgePtr>>{pds, edges};
}


void TCurve::updateSelf(){
    assert(point);
    QString id(point->meshId());
    glm::vec3 worldPos = point->getPos();
    //worldPos = glm::vec3{-15.088, 5.69978, 8.7286};
    //worldPos = glm::vec3{-12.724838256835938, 8.74078369140625, 7.5171};
    //worldPos = glm::vec3{17.648086547851562, 9.5017, 3.8203};
    glm::vec3 worldDir;
    if(id.contains("ring")){
        Ring* r = tee->getRing(id);
        RingAssist ra(*r);
        glm::vec3 localPos = ra.world3DToLocal(worldPos, "pos");
        vector<float> uv = ra.local3DProjectToUV(localPos);
        glm::vec3 localDir = ra.localTangentDir(uv.at(0), uv.at(1), cos(uAng), sin(uAng));
        worldDir = ra.local3DToWorld(localDir, "dir");
    }
    else if(id.contains("cylinder")){
        Cylinder* cy = tee->getCylinder(id);
        CylinderAssist cya(*cy);
        glm::vec3 localPos = cya.world3DToLocal(worldPos, "pos");
        vector<float> uv = cya.local3DProjectToUV(localPos);
        glm::vec3 localDir = cya.localDir(uv.at(0), uv.at(1), cos(uAng), sin(uAng));
        worldDir = cya.local3DToWorld(localDir, "dir");
    }
    else if(id.contains("plane")){
        TriEdgePlane* t = tee->getTriPlane(id);
        TriEdgePlaneAssist ta(*t);
        glm::vec3 localPos = ta.world3DToLocal(worldPos, "pos");
        vector<float> uv = ta.local3DProjectToUV(localPos);
        glm::vec3 localDir = ta.localTangentDir(uv.at(0), uv.at(1), cos(uAng), sin(uAng));
        worldDir = ta.local3DToWorld(localDir, "dir");
    }
    else{
        assert(0);
    }
    PosDir pd{worldPos, worldDir};
    auto posDirs = genCurve(pd, lambda, id, QString(""));
    std::vector<glm::vec3> points;
    for(auto i = posDirs.first.rbegin(); i!=posDirs.first.rend(); i++){
        points.push_back(i->pos);
    }
    PosDir pd2{worldPos, -1.0f*worldDir};
    auto posDir2 = genCurve(pd2, lambda, id, QString(""));
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

bool TCurve::sameEdge(EdgePtr p1, EdgePtr p2){
    if(p1->Id() == p2->Id()){
        return true;
    }
    else if(tee->topoValue(p1->Id()) == p2->Id()){
        return true;
    }
    else{
        return false;
    }
}
