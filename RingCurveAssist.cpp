#include "RingCurveAssist.h"
#include "numpy.h"
#include <Eigen/Dense>
#include <utility>
#include "Edge.h"
using namespace Eigen;

vector<float> RingCurveAssist::ringDiff(float s, vector<float> y0, float lambda){
    float theta = y0.at(0);
    float phi = y0.at(1);
    float alpha = y0.at(2);
    vector<float> y(3);
    y.at(0) = cos(alpha)/(R+r*cos(phi));
    y.at(1) = sin(alpha)/r;
    y.at(2) = lambda*(-(cos(phi)*pow(cos(alpha),2))/(R+r*cos(phi)) - pow(sin(alpha),2)/r)-
            (sin(phi)*cos(alpha))/(R+r*cos(phi));
    return y;
} 

RingCurveAssist::RingCurveAssist(Ring& ring):assist(ring)
{
    initial(ring);
}
void RingCurveAssist::initial(Ring &ring){
    R = ring.R;
    r = ring.r;
    angle = ring.angle;
    anchor = ring.anchor;
    zdir = ring.zdir;
    xdir = ring.xdir;
    id = ring.getId();
    edges = ring.edges;
}



pair<vector<CPPara>, vector<EdgePtr>> RingCurveAssist::genCurve(CPPara p, float coe){
    vector<CPPara> paras;
    float u = p.u;
    float v = p.v;
    float uAng = p.uAng;
    Ys y0{u, v, uAng};
    if(u < 0|| u > angle){
        return pair<vector<CPPara>, vector<EdgePtr>>();
    }

    float length = (R+r)*1000;
    vector<float> xspan1{0, length};
    auto paras1 = rungeKutta(xspan1, y0, coe);
    刚刚把rungeKutta中加入了length,下面将对这个函数加入length，length代表未完成的长度。
    这个函数还是不要动返回值了，动的是另一个genCurve的返回值。
    for(auto i = paras1.first.begin(); i!=paras1.first.end(); i++){
        paras.push_back(*i);
    }
    vector<EdgePtr> edges{nullptr, paras1.second};
    pair<vector<CPPara>, vector<EdgePtr>> result{paras, edges};
    return result;
}

tuple<vector<CPPara>, EdgePtr, float> RingCurveAssist::rungeKutta(vector<float> xspan, CPPara p0, float lam){
    using namespace utility;
    tuple<vector<CPPara>, EdgePtr, float> result;
    float allLength = abs(xspan.at(1) - xspan.at(0));
    flaot length = 0;
    vector<CPPara> para;
    para.push_back(p0);
    float spanL = xspan.at(1)-xspan.at(0);
    unsigned int sign = spanL>0?1:-1;
    float h = sign*0.1;
    float x_last = xspan.at(0);
    Ys y_last = Ys{y0.at(0), y0.at(1), y0.at(2)};
    while(length < allLength){
        Ys y_temp = evalNextRunge(x_last, y_last, h, lam);
        float x_temp = x_last +h;
        float u = y_temp.at(0);
        float v = y_temp.at(1);
        float uAng = y_temp.at(2);
        for(auto e:edges){
            if(e->isOut(u, v)){
                CPPara last = e->extend(para.back(), CPPara{u, v, uAng});
                float l = length(para.bac(), last);
                length = length + l;
                para.push_back(last);
                result = {para, e, allLength - sign*length};
                return result;
            }
        }
        length = length + abs(h);
        if(allLength - length < h){
            h = sign*(allLength - length);
        }
        para.push_back(CPPara{u, v, uAng});
        x_last = x_temp;
        y_last = y_temp;
    }
    resutl = {para, EdgePtr{nullptr, nullptr}, 0};
    return result;
}


vector<float> RingCurveAssist::evalNextRunge(float x, CPPara p0, float h, float coe){
    vector<float> y0{p0.u, p0.v, p0.uAng};
    using namespace numpy;
    vector<float> K1(3);
    vector<float> K2(3);
    vector<float> K3(3);
    vector<float> K4(3);
    K1 = ringDiff(x,y0, coe);
    K2 = ringDiff(x+h/2, vPlus(y0, vMul(K1, h/2)), coe);
    K3 = ringDiff(x+h/2, vPlus(y0, vMul(K2, h/2)), coe);
    K4 = ringDiff(x+h, vPlus(y0, vMul(K3, h)), coe);
    vector<float> y = vPlus(y0, vMul( vPlus(K1, vPlus(vMul(K2, 2), vPlus(vMul(K3, 3), K4))), h/6), coe);
    return y;
}


  pair<std::vector<PosDir>, vector<EdgePtr>> RingCurveAssist::genCurve(glm::vec3 pos, glm::vec3 dir, float coe){
    glm::vec3 localPos = assist.world3DToLocal(pos, "pos");
    glm::vec3 localDir = assist.world3DToLocal(dir, "dir");
    CPPara p1 = assist.local3DProjectToPara(localPos, localDir);
    auto paras = genCurve(p1, coe);
    vector<PosDir> pdVec;
    for(auto i:paras.first){
        auto localPd = assist.CPParaToLocal(i);
        auto worldPos = assist.local3DToWorld(localPd.pos, "pos");
        auto worldDir = assist.local3DToWorld(localPd.dir, "dir");
        pdVec.push_back(PosDir{worldPos, worldDir});
    }
    return  pair<std::vector<PosDir>, vector<EdgePtr>>{pdVec, paras.second};
}
  pair<std::vector<PosDir>, vector<EdgePtr>> RingCurveAssist::genCurve(glm::vec3 pos, float uAng, float coe){
    glm::vec3 localPos = assist.world3DToLocal(pos, "pos");
    auto uv = assist.local3DProjectToUV(localPos);
    CPPara para{uv.at(0), uv.at(1), uAng};
    auto paras = genCurve(para, coe);
    vector<PosDir> pdVec;
    for(auto i:paras.first){
        auto localPd = assist.CPParaToLocal(i);
        auto worldPos = assist.local3DToWorld(localPd.pos, "pos");
        auto worldDir = assist.local3DToWorld(localPd.dir, "dir");
        pdVec.push_back(PosDir{worldPos, worldDir});
    }
    return  pair<std::vector<PosDir>, vector<EdgePtr>>{pdVec, paras.second};
}

  tuple<PosDirVec,EdgePtrVec, float> RingCurveAssist::genCurve(glm::vec3 pos, glm::vec3 dir, float coe, float length){

  }
