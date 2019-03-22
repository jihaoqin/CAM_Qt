#include "TeeCurveAssist.h"
#include "Tee.h"
#include "RingCurveAssist.h"
#include "CyCurveAssist.h"
#include "TriCurveAssist.h"

using namespace std;
TeeCurveAssist::TeeCurveAssist(TeePtr t):tee(t)
{

}

tuple<PosDirVec, QStringVec, EdgePtrVec> TeeCurveAssist::genCurve(PosDir pd, float coe, QString meshId, float length){
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
        return tuple<PosDirVec, QStringVec, EdgePtrVec>{pdM, strs, EdgePtrVec{nullptr, nullptr}};
    }
    else {
        bool flag1 = length>0? true:false;
        bool flag2 = leftLength>0? true:false;
        assert(flag1 == flag2);
        QString topo = tee->allTopoValue(edges.at(1)->Id());
        QString nextMesh = topo.left(topo.indexOf("_"));
        auto next = genCurve(pdM.back(), coe, nextMesh, leftLength);
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
        return tuple<PosDirVec, QStringVec, EdgePtrVec>{pds, strs, EdgePtrVec{nullptr, nextEdge.at(1)}};
    }
}
