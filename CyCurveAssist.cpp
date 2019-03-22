#include "CyCurveAssist.h"
#include "numpy.h"

CyCurveAssist::CyCurveAssist(Cylinder& c):assist(c), anchor(c.anchor) ,zDir(c.zDir), xDir(c.xDir),
    length(c.length), R(c.R), angle(c.angle), edges(c.edges)
{

}


pair<vector<CPPara>, vector<EdgePtr>> CyCurveAssist::genCurve(CPPara p, float coe){
    lambda = coe;
    auto pair1 = rungeKutta(p,"plus",coe);
    vector<EdgePtr> e;
    e.push_back(nullptr);
    e.push_back(pair1.second);
    vector<CPPara> paras;
    auto &para1 = pair1.first;
    for(auto i = para1.begin(); i!=para1.end(); i++){
        paras.push_back(*i);
    }
    return pair<vector<CPPara>, vector<EdgePtr>>{paras, e};
}

pair<vector<CPPara>, EdgePtr> CyCurveAssist::rungeKutta(CPPara p, QString s, float coe){
    int sign = s.contains("plus")? 1:-1;
    float h = sign*length/100.0;
    float s_last;
    CPPara p_last;
    p_last = p;
    s_last = 0;
    EdgePtr side;
    vector<CPPara> paras;
    paras.push_back(p);
    while(1){
        CPPara next = evalNext(s_last, p_last, coe, h);
        //qDebug()<<"v = "<<next.v;
        for(auto e:edges){
            if(e->isOut(next.u, next.v)){
                CPPara m = e->extend(paras.back(), next);
                paras.push_back(m);
                return pair<vector<CPPara>, EdgePtr>{paras, e};
            }
        }
        paras.push_back(next);
        s_last = s_last + h;
        p_last = next;
    }
}

CPPara CyCurveAssist::evalNext(float x, CPPara p, float coe, float h){
    vector<float> y0{p.u, p.v, p.uAng};
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
    return vector<float>{un, vn, uAngn};
}

pair<vector<PosDir>, vector<EdgePtr>> CyCurveAssist::genCurve(glm::vec3 pos, glm::vec3 dir, float coe){
    lambda = coe;
    glm::vec3 localPos = assist.world3DToLocal(pos, "pos");
    glm::vec3 localDir = assist.world3DToLocal(dir, "dir");
    CPPara para = assist.local3DProjectToPara(localPos, localDir);
    auto p = genCurve(para, coe);
    auto paras = p.first;
    vector<PosDir> pds;
    for(auto i:paras){
        pds.push_back(assist.paraToWorld(i));
    }
    return pair<vector<PosDir>, vector<EdgePtr>>{pds, p.second};
}

pair<vector<PosDir>, vector<EdgePtr>> CyCurveAssist::genCurve(glm::vec3 pos, float uAng, float coe){
    lambda = coe;
    glm::vec3 localPos = assist.world3DToLocal(pos, "pos");
    CPPara para = assist.local3DProjectToPara(localPos, glm::vec3{0,0,1});
    para.uAng = uAng;
    auto p = genCurve(para, coe);
    auto paras = p.first;
    vector<PosDir> pds;
    for(auto i:paras){
        pds.push_back(assist.paraToWorld(i));
    }
    return pair<vector<PosDir>, vector<EdgePtr>>{pds, p.second};
}
