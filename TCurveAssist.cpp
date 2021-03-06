#include "TCurveAssist.h"
#include "Tee.h"
#include "RingAssist.h"
#include "RingCurveAssist.h"
#include "CylinderAssist.h"
#include "CyCurveAssist.h"
#include "TriEdgePlaneAssist.h"
#include "TriCurveAssist.h"
using  std::get;

TCurveAssist::TCurveAssist(TeePtr t):tee(t)
{

}
std::tuple<PosDirVec, QStringVec, EdgePtrVec> TCurveAssist::genCurve(Pos p, float uAng, float coe, QString meshId){
    QString id = meshId;
    glm::vec3 worldPos = p;
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
    auto posDirs = genCurve(pd, coe, id);
    return posDirs;
}

std::tuple<PosDirVec, QStringVec, EdgePtrVec> TCurveAssist::genCurve(PosDir pd, float coe, QString meshId){
    tuple<vector<PosDir>,vector<EdgePtr>, float> middle;
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
    PosDirVec& middlePds = get<0>(middle);
    EdgePtrVec& middleEdges = get<1>(middle);
    PosDir side2 = middlePds.back();
    EdgePtr edge2 = middleEdges.back();

    QString val2 = tee->topoValue(edge2->Id());
    QString nextMesh;
    std::tuple<vector<PosDir>,QStringVec, vector<EdgePtr>> backTuple;
    if(!val2.isEmpty()){
        int i = val2.indexOf("_");
        nextMesh = val2.left(i);
        backTuple = genCurve(side2, coe, nextMesh);
    }
    else{
        nextMesh = meshId;
        backTuple = std::tuple<vector<PosDir>, QStringVec, vector<EdgePtr>>
                (PosDirVec{side2, side2}, QStringVec{meshId, meshId}, EdgePtrVec{edge2, edge2});
    }
    vector<PosDir> pds;
    vector<EdgePtr> edges;
    QStringVec strs;

    for(auto &i:middlePds){
        pds.push_back(i);
        strs.push_back(meshId);
    }
    edges.push_back(middleEdges.front());

    PosDirVec &pd2 = get<0>(backTuple);
    QStringVec &str2 = get<1>(backTuple);
    for(unsigned int i=0; i < pd2.size(); i++){
        pds.push_back(pd2.at(i));
        strs.push_back(str2.at(i));
    }
    edges.push_back(std::get<2>(backTuple).back());

    return std::tuple<PosDirVec, QStringVec, EdgePtrVec>{pds, strs, edges};
}

bool TCurveAssist::sameEdge(EdgePtr p1, EdgePtr p2){
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


std::tuple<PosDirVec, QStringVec, EdgePtrVec> TCurveAssist::genDoubleCurve(Pos pd, float uAng, float coe, QString meshId){
    float pi = asin(1)*2;
    auto forwardTuple = genCurve(pd, uAng, coe, meshId);
    auto backTuple = genCurve(pd, uAng+pi, coe, meshId);
    PosDirVec pds;
    QStringVec strs;
    EdgePtrVec edges;
    auto& backPd = get<0>(backTuple);
    auto& backStr = get<1>(backTuple);
    for(int i = backPd.size()-1; i > -1; i--){
        pds.push_back({backPd.at(i).pos, -1.0f*backPd.at(i).dir});
        strs.push_back(backStr.at(i));
    }
    auto& forwardPd = get<0>(forwardTuple);
    auto& forwardStr = get<1>(forwardTuple);
    for(int i = 0; i < forwardPd.size(); i++){
        pds.push_back(forwardPd.at(i));
        strs.push_back(forwardStr.at(i));
    }
    auto& backEdge = get<2>(backTuple);
    auto& forwardEdge = get<2>(forwardTuple);
    edges.push_back(backEdge.at(1));
    edges.push_back(forwardEdge.at(1));
    return tuple<PosDirVec, QStringVec, EdgePtrVec>{pds, strs, edges};
}
