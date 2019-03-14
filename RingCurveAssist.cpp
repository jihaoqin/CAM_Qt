#include "RingCurveAssist.h"
#include "numpy.h"
#include <Eigen/Dense>
using namespace Eigen;

vector<float> RingCurveAssist::ringDiff(float x, vector<float> y0){
    float alpha = y0.at(0);
    float phi = y0.at(1);
    vector<float> y(2);
    y.at(0) = lambda*(-cos(phi)*cos(alpha)-(sin(alpha)*tan(alpha)*(R+r*cos(phi))) / r )-sin(phi);
    y.at(1) = (R+r*cos(phi))/r*tan(alpha);
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
}



std::vector<CPPara> RingCurveAssist::genCurve(CPPara p){
    vector<CPPara> paras;
    float x_begin = p.u;
    Ys y0{p.uAng, p.v};
    if(x_begin <0||x_begin>angle){
        return paras;
    }
    if(abs(x_begin - 0) < 1e-1){
        //xspan=[x_begin, angle];
        vector<float> xspan{x_begin, (float)angle};
        paras = rungeKutta(xspan, y0);
    }
    else if(abs(x_begin -angle)<1e-1){
        vector<float> xspan{x_begin, 0};
        paras = rungeKutta(xspan, y0);
    }
    else {
        //xspan = [x_begin 0];
        vector<float> xspan1{x_begin, 0};
        auto paras1 = rungeKutta(xspan1, y0);
        //xspan = [x_begin angle];
        vector<float> xspan2{x_begin, (float)angle};
        auto paras2 = rungeKutta(xspan2, y0);
        for(auto i = paras1.rbegin(); i!=paras1.rend(); i++){
            paras.push_back(*i);
        }
        for(auto i : paras2){
            paras.push_back(i);
        }
    }
    return paras;
}

vector<CPPara> RingCurveAssist::rungeKutta(vector<float> xspan, Ys y0){
    using namespace utility;
    /*
    float pi = utility::PI;
    float error_goal = 1e-2;
    vector<float> xspan(2);
    xspan.at(0) = 0;
    xspan.at(1) = angle;
    float h = (xspan.at(1) - xspan.at(0))/2;
    vector<float> x;
    vector<Ys> y;
    x.push_back(0);
    y.push_back(Ys{p1.uAng, p1.v});
    while(true){
        float x_last = x.back();
        Ys y_last = y.back();

        Ys y_temp = evalNextRunge(x_last, y_last, h);
        Ys y_twice = evalNextRunge(x_last, y_last, 2*h);
        if(abs(y_twice.at(0)-y_temp.at(0)) < error_goal){
            while(1){
                h = 2*h;
                y_temp = y_twice;
                y_twice = evalNextRunge(x_last, y_last, 2*h);
                if(abs(y_twice.at(0)-y_temp.at(0)) > error_goal){
                    break;
                }
            }
        }
        else{
            Ys y_half = evalNextRunge(x_last, y_last, h/2);
            while(abs(y_temp.at(0) - y_half.at(0)) > error_goal){
                h = h/2;
                y_temp = y_half;
                y_half = evalNextRunge(x_last, y_last, h/2);
            }
        }


    }
    */
    float x_begin = xspan.at(0);
    vector<float> x;
    x.push_back(x_begin);
    vector<CPPara> para;
    para.push_back(CPPara{x_begin, y0.at(1), y0.at(0)});
    float spanL = xspan.at(1)-xspan.at(0);
    int sign = spanL>0? 1:-1;
    float h = sign*1e-2;
    float x_last = x_begin;
    Ys y_last = Ys{y0.at(0), y0.at(1)};
    while(true){
        Ys y_temp = evalNextRunge(x_last, y_last, h);
        float x_temp = x_last +h;
        if(abs(y_temp.at(0))>0.5*PI){
            return para;
        }
        else{
            float xmin = xspan.at(0)<xspan.at(1)? xspan.at(0):xspan.at(1);
            float xmax = xspan.at(0)<xspan.at(1)? xspan.at(1):xspan.at(0);
            float theta = x_temp;
            float alpha = y_temp.at(1);
            if(theta>xmax || theta<xmin){
                return para;
            }
            while(alpha >2*PI){
                alpha = alpha -2*PI;
            }
            while(alpha < 0*PI){
                alpha = alpha + 2*PI;
            }
            if(alpha < 0.5*PI || alpha > 1.5*PI){
                return para;
            }
            para.push_back(CPPara{theta, y_temp.at(1), y_temp.at(0)});
            x_last = x_temp;
            y_last = y_temp;
        }
    }
}


vector<float> RingCurveAssist::evalNextRunge(float x, vector<float> y0, float h){
    using namespace numpy;
    vector<float> K1(y0.size());
    vector<float> K2(y0.size());
    vector<float> K3(y0.size());
    vector<float> K4(y0.size());
    Eigen::VectorXd a;
    K1 = ringDiff(x,y0);
    K2 = ringDiff(x+h/2, vPlus(y0, vMul(K1, h/2)));
    K3 = ringDiff(x+h/2, vPlus(y0, vMul(K2, h/2)));
    K4 = ringDiff(x+h, vPlus(y0, vMul(K3, h)));
    vector<float> y = vPlus(y0, vMul( vPlus(K1, vPlus(vMul(K2, 2), vPlus(vMul(K3, 3), K4))), h/6));
    return y;
}


std::vector<PosDir> RingCurveAssist::genCurve(glm::vec3 pos, glm::vec3 dir, float coe){
    lambda = coe;
    glm::vec3 localPos = assist.world3DToLocal(pos, "pos");
    glm::vec3 localDir = assist.world3DToLocal(dir, "dir");
    CPPara p1 = assist.local3DProjectToPara(localPos, localDir);
    auto paras = genCurve(p1);
    vector<PosDir> pdVec;
    for(auto i:paras){
        auto localPd = assist.CPParaToLocal(i);
        auto worldPos = assist.local3DToWorld(localPd.pos, "pos");
        auto worldDir = assist.local3DToWorld(localPd.dir, "dir");
        pdVec.push_back(PosDir{worldPos, worldDir});
    }
    return pdVec;
}

std::vector<PosDir> RingCurveAssist::genCurve(glm::vec3 pos, float uAng, float coe){
    //pos = glm::vec3{11.66718, 14.389130, -0.95986};
    lambda = coe;
    glm::vec3 localPos = assist.world3DToLocal(pos, "pos");
    auto uv = assist.local3DProjectToUV(localPos);
    CPPara para{uv.at(0), uv.at(1), uAng};
    auto paras = genCurve(para);
    vector<PosDir> pdVec;
    for(auto i:paras){
        auto localPd = assist.CPParaToLocal(i);
        auto worldPos = assist.local3DToWorld(localPd.pos, "pos");
        auto worldDir = assist.local3DToWorld(localPd.dir, "dir");
        pdVec.push_back(PosDir{worldPos, worldDir});
    }
    return pdVec;
}
