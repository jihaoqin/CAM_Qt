#include "TCurve.h"
#include "Point.h"
#include "Tee.h"
#include "RingAssist.h"
#include "CPPara.h"
#include "RingCurveAssist.h"

TCurve::TCurve(PointPtr p, float a, float coe, const char*c , TeePtr t)
    :Curve(c),designP(p), uAng(a), lambda(coe), tee(t)
{

}

std::pair<vector<PosDir>, vector<EdgePtr>> TCurve::genCurve(PosDir pd, QString meshId){
    /*
    std::pair<vector<PosDir>,vector<EdgePtr>> front;
    std::pair<vector<PosDir>,vector<EdgePtr>> middle;
    std::pair<vector<PosDir>,vector<EdgePtr>> back;
    if(meshId.contains("ring")){
        RingCurveAssist assist;
        middle = assist.genCurve();
    }
    else{
        assert(0);
    }
    PosDir side1 = middle.first.front();
    EdgePtr edge1 = middle.second.front();
    PosDir side2 = middle.first.back();
    EdgePtr edge2 = middle.second.back();

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
}
