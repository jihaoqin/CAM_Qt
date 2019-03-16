#include "TCurve.h"
#include "Point.h"
#include "Tee.h"
#include "RingAssist.h"
#include "CPPara.h"
#include "RingCurveAssist.h"

TCurve::TCurve(PointPtr p, float a, float coe, const char*c , TeePtr t)
    :Curve(c),designP(p), uAng(a), lambda(coe), tee(t)
{

    /*
    PosDir p1, p2;
    QString meshType(p->meshId());
    if(meshType.contains("ring")){
        Ring* r = tee->getRing(meshType);
        assert(r);
        RingAssist assist(*r);
        vector<float> uv = assist.paraWithLocalPoint(designP->getPos());
        CPPara para{uv.at(0), uv.at(1), uAng};
        PosDir pd = assist.worldPosDir(para);
    }
    else if(meshType.contains("plane")){

    }
    else if(meshType.contains("cylinder")){

    }
    p1 , p2 =;
    nextMeshName = tAssist.nextMesh( meshName_p1, edgeName_p1);
    curveVec.push_back(c);
    if(nextMeshName == outMeshName){
        return;
    }
    else{
        pushCurve(p1, meshName);
        pushCurve(p2, meshName);
    }
    */
}

void TCurve::genCurve(PosDir pd, QString meshId){
    /*
    if(meshId.contains("ring")){
        RingCurveAssist assist;
        result = assist.genCurve();
        push result;
    }
    else{
        assert(0);
    }
    side1, edge1, side2, edge2;
    if(!edge1.isBoundary){
        mesh1 = edge1, meshId;
        nextMesh = edge1, mesh1;
        if(nextMesh != meshId){
            genCurve(side1, mesh1);
        }
    }
    if(!edge2.isBoundaray){

    }
    tee->getMesh(meshId);
    */
}
