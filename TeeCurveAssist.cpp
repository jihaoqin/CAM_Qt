#include "TeeCurveAssist.h"
#include "Tee.h"
#include "RingCurveAssist.h"
#include "CyCurveAssist.h"
#include "TriCurveAssist.h"

using namespace std;
TeeCurveAssist::TeeCurveAssist(TeePtr t):tee(t)
{

}

tuple<PosDirVec, QStringVec, EdgePtrVec, float> TeeCurveAssist::genCurve(PosDir pd, float coe, QString meshId, float length){
    tuple<PosDirVec,EdgePtrVec, float> middle;
    if(meshId.contains("ring")){
        RingCurveAssist assist(*(tee->getRing(meshId)));
        middle = assist.genCurve(pd.pos, pd.dir, coe, length);
    }
    else if(meshId.contains("cylinder")){
        CyCurveAssist assist(*(tee->getCylinder(meshId)));
        middle = assist.genCurve(pd.pos, pd.dir, coe, length);
    }
    else if(meshId.contains("plane")){
        TriCurveAssist assist(*(tee->getTriPlane(meshId)));
        middle = assist.genCurve(pd.pos, pd.dir, coe, length);
    }
    else{
        assert(0);
    }
    float leftLength = get<2>(middle);
    PosDirVec &pdM = get<0>(middle);
    EdgePtrVec &edges = get<1>(middle);
    if(abs(leftLength) < 1e-2){
        QStringVec strs;
        for(auto &i:pdM){
            strs.push_back(meshId);
        }
        return tuple<PosDirVec, QStringVec, EdgePtrVec, float>{pdM, strs, EdgePtrVec{nullptr, nullptr}, 0};
    }
    else {
        bool flag1 = length>0? true:false;
        bool flag2 = leftLength>0? true:false;
        //assert(flag1 == flag2);
        QString topo = tee->allTopoValue(edges.at(1)->Id());
        if(topo.isEmpty()){
            QStringVec strs;
            for(auto &i:pdM){
                strs.push_back(meshId);
            }
            return tuple<PosDirVec, QStringVec, EdgePtrVec, float>{pdM, strs, EdgePtrVec{nullptr, nullptr}, leftLength};
        }
        else{
            QString nextMesh = topo.left(topo.indexOf("_"));
            auto next = genCurve(pdM.back(), coe, nextMesh, leftLength);
            leftLength = get<3>(next);
            PosDirVec nextPd = get<0>(next);
            QStringVec nextStr = get<1>(next);
            EdgePtrVec nextEdge = get<2>(next);
            PosDirVec pds;
            QStringVec strs;
            for(auto& i:pdM){
                pds.push_back(i);
                strs.push_back(meshId);
            }
            for(unsigned int i = 0; i<nextPd.size(); i++){
                pds.push_back(nextPd.at(i));
                strs.push_back(nextStr.at(i));
            }
            return tuple<PosDirVec, QStringVec, EdgePtrVec, float>{pds, strs, EdgePtrVec{nullptr, nextEdge.at(1)}, leftLength};
        }
    }
}

tuple<PosDirVec, QStringVec, EdgePtrVec, float> TeeCurveAssist::genCurve(Pos p, float uAng, float coe, QString meshId, float length){
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
    auto posDirs = genCurve(pd, coe, id, length);
    return posDirs;
}
