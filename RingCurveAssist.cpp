#include "RingCurveAssist.h"
#include "numpy.h"
#include <Eigen/Dense>
#include <utility>
#include "Edge.h"
using namespace Eigen;
using namespace std;

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



tuple<vector<CPPara>, vector<EdgePtr>, float> RingCurveAssist::genCurve(CPPara p, float coe, float length){
    vector<CPPara> paras;
    float u = p.u;
    float v = p.v;
    if(u < 0|| u > angle){
        assert(0);
    }
    vector<float> xspan1{0, length};
    auto paras1 = rungeKutta(xspan1, p, coe);
    CPParaVec cps1 = std::get<0>(paras1);
    EdgePtr ed1 = std::get<1>(paras1);
    float leftLength = get<2>(paras1);
    for(auto i = cps1.begin(); i!=cps1.end(); i++){
        paras.push_back(*i);
    }
    vector<EdgePtr> edges{nullptr, ed1};
    tuple<vector<CPPara>, vector<EdgePtr>, float> result{paras, edges, leftLength};
    return result;
}

tuple<vector<CPPara>, EdgePtr, float> RingCurveAssist::rungeKutta(vector<float> xspan, CPPara p0, float lam){
    using namespace utility;
    tuple<vector<CPPara>, EdgePtr, float> result;
    float allLength = abs(xspan.at(1) - xspan.at(0));
    float length = 0;
    vector<CPPara> para;
    para.push_back(p0);
    float spanL = xspan.at(1)-xspan.at(0);
    unsigned int sign = spanL>0?1:-1;
    float h = sign*0.1;
    float x_last = xspan.at(0);
    CPPara y_last{p0.u, p0.v, p0.uAng};
    while(length < allLength){
        CPPara y_temp = evalNextRunge(x_last, y_last, h, lam);
        float x_temp = x_last +h;
        for(auto e:edges){
            if(e->isOut(y_temp.u, y_temp.v)){
                CPPara last = e->extend(para.back(), y_temp);
                float l = length3D(para.back(), last);
                length = length + l;
                para.push_back(last);
                result = {para, e, sign*(allLength - length)};
                return result;
            }
        }
        length = length + abs(h);
        if(allLength - length < abs(h)){
            h = sign*(allLength - length);
        }
        para.push_back(y_temp);
        x_last = x_temp;
        y_last = y_temp;
    }
    result = {para, nullptr, 0};
    return result;
}

CPPara RingCurveAssist::evalNextRunge(float x, CPPara p0, float h, float coe){
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
    vector<float> y = vPlus(y0, vMul( vPlus(K1, vPlus(vMul(K2, 2), vPlus(vMul(K3, 3), K4))), h/6));
    CPPara result{y.at(0), y.at(1), y.at(2)};
    return result;
}


tuple<std::vector<PosDir>, vector<EdgePtr>, float> RingCurveAssist::genCurve(glm::vec3 pos, glm::vec3 dir, float coe, float length){
    glm::vec3 localPos = assist.world3DToLocal(pos, "pos");
    glm::vec3 localDir = assist.world3DToLocal(dir, "dir");
    CPPara p1 = assist.local3DProjectToPara(localPos, localDir);
    auto paras = genCurve(p1, coe, length);
    CPParaVec pds = get<0>(paras);
    vector<PosDir> pdVec;
    for(auto i:pds){
        auto localPd = assist.CPParaToLocal(i);
        auto worldPos = assist.local3DToWorld(localPd.pos, "pos");
        auto worldDir = assist.local3DToWorld(localPd.dir, "dir");
        pdVec.push_back(PosDir{worldPos, worldDir});
    }
    return  tuple<PosDirVec, EdgePtrVec, float>{pdVec, get<1>(paras), get<2>(paras)};
}
tuple<std::vector<PosDir>, vector<EdgePtr>, float> RingCurveAssist::genCurve(glm::vec3 pos, float uAng, float coe, float length){
    glm::vec3 localPos = assist.world3DToLocal(pos, "pos");
    auto uv = assist.local3DProjectToUV(localPos);
    CPPara para{uv.at(0), uv.at(1), uAng};
    auto paras = genCurve(para, coe, length);
    CPParaVec pds = get<0>(paras);
    vector<PosDir> pdVec;
    for(auto i:pds){
        auto localPd = assist.CPParaToLocal(i);
        auto worldPos = assist.local3DToWorld(localPd.pos, "pos");
        auto worldDir = assist.local3DToWorld(localPd.dir, "dir");
        pdVec.push_back(PosDir{worldPos, worldDir});
    }
    return  tuple<PosDirVec, EdgePtrVec, float>{pdVec, get<1>(paras), get<2>(paras)};
}


  float RingCurveAssist::length3D(CPPara p1, CPPara p2){
      float du = p2.u - p1.u;
      float dv = p2.v - p1.v;
      float E1 = pow(R+r*cos(p1.v), 2);
      float F1 = 0;
      float G1 = pow(r, 2);
      float E2 = pow(R+r*cos(p2.v), 2);
      float F2 = 0;
      float G2 = pow(r, 2);
      float ds1 = sqrt(E1*du*du + F1*du*dv + G1*du*dv);
      float ds2 = sqrt(E2*du*du + F2*du*dv + G2*du*dv);
      float ds = (ds1+ds2)/2;
      return ds;
  }
