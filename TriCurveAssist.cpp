#include "TriCurveAssist.h"
#include "Edge.h"

using namespace std;
TriCurveAssist::TriCurveAssist(TriEdgePlane& plane)
    :assist(plane), R(plane.R), uDir(plane.uDir), anchor(plane.anchor), norm(plane.norm), edges(plane.edges)
{

}


tuple<PosDirVec, EdgePtrVec, float> TriCurveAssist::genCurve(glm::vec3 pos, glm::vec3 dir, float coe, float length){
    glm::vec3 localPos = assist.world3DToLocal(pos, "pos");
    glm::vec3 localDir = assist.world3DToLocal(dir, "dir");
    CPPara para = assist.local3DProjectToPara(localPos, localDir);
    auto result = genCurve(para, coe, length);
    auto paras = get<0>(result);
    vector<PosDir> pds;
    for(auto i:paras){
        auto p = assist.CPParaToLocal(i);
        glm::vec3 worldPos = assist.local3DToWorld(p.pos, "pos");
        glm::vec3 worldDir = assist.local3DToWorld(p.dir, "dir");
        pds.push_back(PosDir{worldPos, worldDir});
    }
    return tuple<PosDirVec, EdgePtrVec, float>{pds, get<1>(result), get<2>(result)};
}

tuple<PosDirVec, EdgePtrVec, float> TriCurveAssist::genCurve(glm::vec3 pos, float uAng, float coe, float length){
    glm::vec3 localPos = assist.world3DToLocal(pos, "pos");
    vector<float> uv = assist.local3DProjectToUV(localPos);
    float u = uv.at(0);
    float v = uv.at(1);
    CPPara para{u, v, uAng};
    auto result = genCurve(para, coe, length);
    auto paras = get<0>(result);
    vector<PosDir> pds;
    for(auto i:paras){
        auto p = assist.CPParaToLocal(i);
        glm::vec3 worldPos = assist.local3DToWorld(p.pos, "pos");
        glm::vec3 worldDir = assist.local3DToWorld(p.dir, "dir");
        pds.push_back(PosDir{worldPos, worldDir});
    }
    return tuple<PosDirVec, EdgePtrVec, float>{pds, get<1>(result), get<2>(result)};
}

tuple<CPParaVec, EdgePtrVec, float> TriCurveAssist::genCurve(CPPara p, float coe, float length){
    float pi = asin(1)*2;
    float uAng = p.uAng;
    int sign = length>0?1:-1;
    float ds = sign*0.1;
    float u_last = p.u;
    float v_last = p.v;
    vector<CPPara> paras{p};
    EdgePtr edge1;
    float l = 0;
    float allLength = abs(length);
    while(l < allLength){
        float u_temp = u_last+ds*cos(uAng);
        float v_temp = v_last+ds*sin(uAng);
        for(auto e:edges){
            if(e->isOut(u_temp,v_temp)){
                CPPara p_temp{u_temp, v_temp, uAng};
                CPPara p_last{u_last, v_last, uAng};
                CPPara next = e->extend(p_last, p_temp);
                paras.push_back(next);
                float lastL = length3D(p_last, p_temp);
                l = l + lastL;
                edge1 = e;
                return tuple<CPParaVec, EdgePtrVec, float>{paras, {nullptr, edge1}, sign*(allLength-l)};
            }
        }
        l = l + abs(ds);
        if(allLength - l < abs(ds)){
            ds = sign*(allLength - l);
        }
        paras.push_back(CPPara{u_temp, v_temp, uAng});
        u_last = u_temp;
        v_last = v_temp;
    }

    vector<EdgePtr> eptr{nullptr, edge1};
    return tuple<CPParaVec, EdgePtrVec, float>{paras, eptr, 0};
}

float TriCurveAssist::length3D(CPPara p1, CPPara p2){
    return sqrt(pow(p2.u - p1.u, 2)+pow(p2.v - p1.v, 2));
}
