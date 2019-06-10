#include "CyCurveAssist.h"
#include "numpy.h"
using namespace std;

CyCurveAssist::CyCurveAssist(Cylinder& c):assist(c), anchor(c.anchor) ,zDir(c.zDir), xDir(c.xDir),
    length(c.length), R(c.R), angle(c.angle), edges(c.edges)
{

}


tuple<vector<CPPara>, vector<EdgePtr>, float> CyCurveAssist::genCurve(CPPara p, float coe, float l){
    l=l;
    lambda = coe;
    auto tuple1 = rungeKutta(l, p,coe);
    vector<EdgePtr> e;
    e.push_back(nullptr);
    e.push_back(get<1>(tuple1));
    vector<CPPara> paras;
    auto &para1 = get<0>(tuple1);
    for(auto i = para1.begin(); i!=para1.end(); i++){
        paras.push_back(*i);
    }
    return tuple<CPParaVec, EdgePtrVec, float>{paras, e, get<2>(tuple1)};
}

tuple<CPParaVec, EdgePtr, float> CyCurveAssist::rungeKutta(float sSpan, CPPara p,  float coe){
    int sign = sSpan>0? 1:-1;
    //间隔
    float h = sign*0.1;
    float s_last;
    CPPara p_last;
    p_last = p;
    s_last = 0;
    EdgePtr side;
    vector<CPPara> paras;
    paras.push_back(p);
    float l = 0;
    float allLength = abs(sSpan);
    while(l<allLength){
        CPPara next = evalNext(s_last, p_last, coe, h);
        for(auto e:edges){
            if(e->isOut(next.u, next.v)){
                CPPara m = e->extend(paras.back(), next);
                float lastL = length3D(paras.back(), next);
                l = l + lastL;
                paras.push_back(m);
                return tuple<vector<CPPara>, EdgePtr, float>{paras, e, sign*(allLength - l)};
            }
        }
        l = l + abs(3.5/3*h);
        if(allLength - l < abs(h)){
            h = sign*(allLength - l);
        }
        paras.push_back(next);
        s_last = s_last + h;
        p_last = next;
    }
    return tuple<CPParaVec, EdgePtr, float>{paras, nullptr, 0};
}

CPPara CyCurveAssist::evalNext(float x, CPPara p, float coe, float h){
    //vector<float> y0{p.u, p.v, p.uAng};
    vector<float> y0;
    y0.push_back(p.u);
    y0.push_back(p.v);
    y0.push_back(p.uAng);
    using namespace numpy;
    vector<float> K1(y0.size());
    vector<float> K2(y0.size());
    vector<float> K3(y0.size());
    vector<float> K4(y0.size());
    K1 = ringDiff(x,y0, coe);
    K2 = ringDiff(x+h/2, vPlus(y0, vMul(K1, h/2)), coe);
    K3 = ringDiff(x+h/2, vPlus(y0, vMul(K2, h/2)), coe);
    K4 = ringDiff(x+h, vPlus(y0, vMul(K3, h)), coe);
    vector<float> y = vPlus(y0, vMul( vPlus(K1, vPlus(vMul(K2, 2), vPlus(vMul(K3, 3), K4))), h/6));
    return CPPara{y.at(0), y.at(1), y.at(2)};
}

vector<float> CyCurveAssist::ringDiff(float s, vector<float> y0, float coe){
    float u = y0.at(0);
    float v = y0.at(1);
    float uAng = y0.at(2);
    float uAngn = -1*coe*pow(cos(uAng),2)/R;
    float un = cos(uAng)/R;
    float vn = sin(uAng);
    vector<float> result;
    result.push_back(un);
    result.push_back(vn);
    result.push_back(uAngn);
    return result;
    //return vector<float>{un, vn, uAngn};
}

tuple<vector<PosDir>, vector<EdgePtr>, float> CyCurveAssist::genCurve(glm::vec3 pos, glm::vec3 dir, float coe, float l){
    lambda = coe;
    glm::vec3 localPos = assist.world3DToLocal(pos, "pos");
    glm::vec3 localDir = assist.world3DToLocal(dir, "dir");
    CPPara para = assist.local3DProjectToPara(localPos, localDir);
    auto p = genCurve(para, coe, l);
    auto paras = get<0>(p);
    vector<PosDir> pds;
    for(auto i:paras){
        pds.push_back(assist.paraToWorld(i));
    }
    return tuple<vector<PosDir>, vector<EdgePtr>, float>{pds, get<1>(p), get<2>(p)};
}

tuple<vector<PosDir>, vector<EdgePtr>, float> CyCurveAssist::genCurve(glm::vec3 pos, float uAng, float coe, float l){
    lambda = coe;
    glm::vec3 localPos = assist.world3DToLocal(pos, "pos");
    CPPara para = assist.local3DProjectToPara(localPos, glm::vec3{0,0,1});
    para.uAng = uAng;
    auto p = genCurve(para, coe, l);
    auto paras = get<0>(p);
    vector<PosDir> pds;
    for(auto i:paras){
        pds.push_back(assist.paraToWorld(i));
    }
    return tuple<vector<PosDir>, vector<EdgePtr>, float>{pds, get<1>(p), get<2>(p)};
}

float CyCurveAssist::length3D(CPPara p1, CPPara p2){
    float du = p2.u - p1.u;
    float dv = p2.v - p2.v;
    return sqrt(pow(R*du, 2) + pow(dv, 2));
}
