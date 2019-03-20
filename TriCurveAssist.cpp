#include "TriCurveAssist.h"
#include "Edge.h"

TriCurveAssist::TriCurveAssist(TriEdgePlane& plane)
    :assist(plane), R(plane.R), uDir(plane.uDir), anchor(plane.anchor), norm(plane.norm), edges(plane.edges)
{

}


std::pair<vector<PosDir>, vector<EdgePtr>> TriCurveAssist::genCurve(glm::vec3 pos, glm::vec3 dir, float coe){
    glm::vec3 localPos = assist.world3DToLocal(pos, "pos");
    glm::vec3 localDir = assist.world3DToLocal(dir, "dir");
    CPPara para = assist.local3DProjectToPara(localPos, localDir);
    //para = CPPara{-12.934589f, 0.0f, 2.242562f};
    auto result = genCurve(para);
    auto paras = result.first;
    vector<PosDir> pds;
    for(auto i:paras){
        auto p = assist.CPParaToLocal(i);
        glm::vec3 worldPos = assist.local3DToWorld(p.pos, "pos");
        glm::vec3 worldDir = assist.local3DToWorld(p.dir, "dir");
        pds.push_back(PosDir{worldPos, worldDir});
    }
    return std::pair<vector<PosDir>, vector<EdgePtr>>{pds, result.second};
}

std::pair<vector<PosDir>, vector<EdgePtr>> TriCurveAssist::genCurve(glm::vec3 pos, float uAng, float coe){
    glm::vec3 localPos = assist.world3DToLocal(pos, "pos");
    vector<float> uv = assist.local3DProjectToUV(localPos);
    float u = uv.at(0);
    float v = uv.at(1);
    CPPara para{u, v, uAng};
    auto result = genCurve(para);
    auto paras = result.first;
    vector<PosDir> pds;
    for(auto i:paras){
        auto p = assist.CPParaToLocal(i);
        glm::vec3 worldPos = assist.local3DToWorld(p.pos, "pos");
        glm::vec3 worldDir = assist.local3DToWorld(p.dir, "dir");
        pds.push_back(PosDir{worldPos, worldDir});
    }
    return std::pair<vector<PosDir>, vector<EdgePtr>>{pds, result.second};
}

 std::pair<std::vector<CPPara>, vector<EdgePtr>> TriCurveAssist::genCurve(CPPara p){
    float pi = asin(1)*2;
    float u = p.u;
    float v = p.v;
    float uAng = p.uAng;
    float ds = 0.1;
    float u_last = u;
    float v_last = v;
    vector<CPPara> para1{CPPara{u, v, uAng}};
    EdgePtr edge1;
    bool flag1 = true;
    while(flag1){
        float u_temp = u_last+ds*cos(uAng);
        float v_temp = v_last+ds*sin(uAng);
        for(auto e:edges){
            if(e->isOut(u_temp,v_temp)){
                edge1 = e;
                flag1 = false;
                break;
            }
        }
        if(flag1 == true){
            para1.push_back(CPPara{u_temp, v_temp, uAng});
            u_last = u_temp;
            v_last = v_temp;
        }
    }

    vector<CPPara> para2{CPPara{u, v, uAng}};
    EdgePtr edge2;
    u_last = u;
    v_last = v;
    bool flag2 = true;
    while(flag2){
        float u_temp = u_last+ds*cos(uAng+pi);
        float v_temp = v_last+ds*sin(uAng+pi);
        for(auto e:edges){
            if(e->isOut(u_temp,v_temp)){
                edge2 = e;
                flag2 = false;
                break;
            }
        }
        if(flag2 == true){
            para2.push_back(CPPara{u_temp, v_temp, uAng});
            u_last = u_temp;
            v_last = v_temp;
        }
    }
    vector<CPPara> paras;
    for(auto i=para1.rbegin(); i!= para1.rend(); i++){
        paras.push_back(*i);
    }
    for(auto i = para2.begin(); i!=para2.end(); i++){
        paras.push_back(*i);
    }

    vector<EdgePtr> eptr{edge1, edge2};
    std::pair<vector<CPPara>, vector<EdgePtr>> result{paras, eptr};
    return result;
}
