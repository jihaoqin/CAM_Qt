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

TCurve::TCurve(PointPtr p, float a, float coe, const char*c , TeePtr t)
    :Curve(c), point(p), uAng(a), lambda(coe), tee(t)
{

}

std::pair<vector<PosDir>, vector<EdgePtr>> TCurve::genCurve(PosDir pd, float coe, QString meshId){
    std::pair<vector<PosDir>,vector<EdgePtr>> front;
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
    else if(meshId.contains("triEdgePlane")){
        TriCurveAssist assist(*(tee->getTriPlane(meshId)));
        middle = assist.genCurve(pd.pos, pd.dir, coe);
    }
    else{
        assert(0);
    }
    PosDir side1 = middle.first.front();
    EdgePtr edge1 = middle.second.front();
    PosDir side2 = middle.first.back();
    EdgePtr edge2 = middle.second.back();

    /*
    if(!edge1.isBoundary()){
        mesh1 = edge1, meshId;
        nextMesh = edge1, mesh1;
        if(nextMesh != meshId){
            front = genCurve(side1, mesh1);
        }
        else
        {
            front =
        }
    }
    else{
        front =
    }
    if(!edge2.isBoundaray){
        mesh2 = edge2, meshId;
        nextMesh = edge2, mesh2;
        if(nextMesh != meshId){
            back = genCurve(side1, mesh1);
        }
    }
    else{
        back =
    }
    vector<PosDir> pds;
    vector<EdgePtr> edges;
    EdgePtr frontEdge1 = front.second.first();
    EdgePtr frontEdge2 = front.second.back();
    if(frontEdge1.id == edge1.id){
        for(auto i = front.first.rbegin(); i!=front.first.rend(); i++){
            pds.push_back(*i);
        }
        edges.push_back(frontEdge2);
        edges.push_back(frontEdge1);
    }
    else if(frontEdge2.id == edge1.id){
        for(auto i = front.first.begin(); i!= front.first.end(); i++){
            pds.push_back(*i);
        }
        edges.push_back(frontEdge1);
        edges.push_back(frontEdge2);
    }
    else{
        assert(0);
    }

    for(auot i:middle.first){
        pds.push_back(i);
    }
    edges.at(1) = middle.second.back();


    EdgePtr backEdge1 = back.second.first();
    EdgePtr backEdge2 = back.second.back();
    if(backEdge1.id == edge2.id){
        for(auto i = back.first.begin(); i!=back.first.end(); i++){
            pds.push_back(*i);
        }
        edges.at(1) = backEdge2;
    }
    else if(backEdge2.id == edge2.id){
        for(auto i = back.first.rbegin(); i!= back.first.rend(); i++){
            pds.push_back(*i);
        }
        edges.at(1) = backEdge1;
    }
    else{
        assert(0);
    }
    return std::pair<vector<PosDir>, vector<EdgePtr>>{pds, edges};
    */
    return std::pair<vector<PosDir>, vector<EdgePtr>>();
}


void TCurve::updateSelf(){
    assert(point);
    QString id(point->meshId());
    glm::vec3 worldPos = point->getPos();
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
    else if(id.contains("triEdgePlane")){
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
    auto posDirs = genCurve(pd, 0.1, id);
    std::vector<glm::vec3> points;
    for(auto i:posDirs.first){
        points.push_back(i.pos);
    }
    data(points);
}
