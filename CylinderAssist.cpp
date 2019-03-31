#include "CylinderAssist.h"
#include "utility.h"
#include <complex>
#include <cmath>
using namespace utility;
using std::complex;
CylinderAssist::CylinderAssist(Cylinder& c):anchor(c.anchor),zDir(c.zDir), xDir(c.xDir),
    length(c.length), angle(c.angle), R(c.R), id(c.getId())
{
    setPos(T, anchor);
    setXDir(T,xDir);
    setYDir(T,glm::cross(zDir, xDir));
    setZDir(T, zDir);
}
vector<float> CylinderAssist::local3DProjectToUV(glm::vec3 pos){
    float pi = asin(1)*2;
    vector<float> uv = local3DToUV(pos);
    float u = uv.at(0);
    float v = uv.at(1);
    while(u<(-1*pi+0.5*angle)){
        u=u+2*pi;
    }
    while(u>(pi+0.5*angle)){
        u=u-2*pi;
    }

    if(v<=0){
        if(u <= 0){
            u = 0;
            v = 0;
        }
        else if(u>0 && u<angle){
            u = u;
            v = 0;
        }
        else{
            u = angle;
            v = 0;
        }
    }
    else if(0<v && v<length){
        if(u<=0){
            u = 0;
            v = v;
        }
        else if(u>0 && u<angle){
            u = u; v =v;
        }
        else{
            u = angle;
            v = v;
        }
    }
    else{
        if(u<=0){
            u = 0;
            v = length;
        }
        else if(u>0 && u<angle){
            u = u;
            v = length;
        }
        else{
            u = angle;
            v = length;
        }
    }

    while(u > pi){
        u = u -2*pi;
    }
    while(u < -1*pi){
        u = u +2*pi;
    }
    return vector<float>{u, v};
}


vector<float> CylinderAssist::local3DToUV(glm::vec3 pos){
    float u = atan2(pos.y, pos.x);
    float v = pos.z;
    return vector<float>{u, v};
}

glm::vec3 CylinderAssist::local3DToWorld(glm::vec3 local, const char* c){
    QString flag(c);
    glm::vec3 world;
    if(flag.contains("pos")){
        world = T*glm::vec4(local, 1);
    }
    else{
        world = T*glm::vec4(local, 0);
    }
    return world;
}

glm::vec3 CylinderAssist::world3DToLocal(glm::vec3 world, const char* c){
    glm::mat4 invT = glm::inverse(T);
    QString flag(c);
    glm::vec3 local;
    if(flag.contains("pos")){
        local = invT*glm::vec4(world, 1);
    }
    else{
        local = invT*glm::vec4(world, 0);
    }
    return local;
}

CPPara CylinderAssist::local3DProjectToPara(glm::vec3 pos, glm::vec3 dir){
    auto uv = local3DProjectToUV(pos);
    float u = uv.at(0);
    float v = uv.at(1);
    glm::vec3 uTan = localDir(u, v, 1, 0);
    glm::vec3 vTan = localDir(u, v, 0, 1);
    glm::vec3 c = glm::cross(uTan, vTan);
    c = glm::normalize(c);
    glm::vec3 part = dir - glm::dot(dir,c)*c;
    float uAng = atan2(glm::dot(vTan, part), glm::dot(uTan, part));
    return CPPara{u, v, uAng};
}

glm::vec3 CylinderAssist::localDir(float u, float v, float uW, float vW){
    glm::vec3 uTan{-1*R*sin(u), R*cos(u), 0};
    uTan = glm::normalize(uTan);
    glm::vec3 vTan{0,0,1};
    return glm::normalize(uW*uTan + vW*vTan);
}

PosDir CylinderAssist::paraToWorld(CPPara a){
    float u = a.u;
    float v = a.v;
    float uAng = a.uAng;
    glm::vec3 pos{R*cos(u), R*sin(u), v};
    glm::vec3 dir = localDir(u, v, cos(uAng), sin(uAng));
    glm::vec3 worldPos = local3DToWorld(pos, "pos");
    glm::vec3 worldDir = local3DToWorld(dir, "dir");
    return PosDir{worldPos, worldDir};
}

vector<EdgePtr> CylinderAssist::getEdges(){
    float pi = asin(1)*2;
    float aangle = angle;
    auto edge1 = [pi, aangle](float u, float v)->bool{
        while(u>pi+0.5*aangle){
            u = u-2*pi;
        }
        while(u<0.5*aangle-pi){
            u = u+2*pi;
        }
        return u<0? true:false;
    };
    auto ex1 = [pi, aangle](CPPara p1, CPPara p2)->CPPara{
        while(p1.u>pi+0.5*aangle){
            p1.u = p1.u-2*pi;
        }
        while(p1.u<0.5*aangle-pi){
            p1.u = p1.u+2*pi;
        }
        while(p2.u>pi+0.5*aangle){
            p2.u = p2.u-2*pi;
        }
        while(p2.u<0.5*aangle-pi){
            p2.u = p2.u+2*pi;
        }
        float lam = (0 - p1.u)/(p2.u - p1.u);
        return CPPara{0, (1-lam)*p1.v+lam*p2.v,(1-lam)*p1.uAng+lam*p2.uAng};
    };
    auto edge2 = [pi, aangle](float u, float v)->bool{
        while(u>pi+0.5*aangle){
            u = u-2*pi;
        }
        while(u<0.5*aangle-pi){
            u = u+2*pi;
        }
        return u>aangle? true:false;
    };
    auto ex2 = [pi, aangle](CPPara p1, CPPara p2)->CPPara{
        while(p1.u>pi+0.5*aangle){
            p1.u = p1.u-2*pi;
        }
        while(p1.u<0.5*aangle-pi){
            p1.u = p1.u+2*pi;
        }
        while(p2.u>pi+0.5*aangle){
            p2.u = p2.u-2*pi;
        }
        while(p2.u<0.5*aangle-pi){
            p2.u = p2.u+2*pi;
        }
        float lam = (aangle - p1.u)/(p2.u - p1.u);
        return CPPara{aangle, (1-lam)*p1.v+lam*p2.v,(1-lam)*p1.uAng+lam*p2.uAng};
    };
    auto edge3 = [](float u, float v)->bool{
        return v<0? true:false;
    };
    auto ex3 = [pi, aangle](CPPara p1, CPPara p2)->CPPara{
        float lam = (0 - p1.v)/(p2.v - p1.v);
        return CPPara{(1-lam)*p1.u+lam*p2.u, 0, (1-lam)*p1.uAng+lam*p2.uAng};
    };
    float l = length;
    auto edge4 = [l](float u, float v)->bool{
        return v>l? true:false;
    };
    auto ex4 = [l, pi, aangle](CPPara p1, CPPara p2)->CPPara{
        float lam = (l - p1.v)/(p2.v - p1.v);
        return CPPara{(1-lam)*p1.u+lam*p2.u, l, (1-lam)*p1.uAng+lam*p2.uAng};
    };
    EdgePtr e1 = std::make_shared<Edge>(edge1);
    e1->Id(id+"_edge1");
    e1->setExtend(ex1);
    vector<glm::vec3> ps1;
    for(int i =0; i<10; i++){
        float u = 0;
        float v = length/9*i;
        ps1.push_back(paraToWorld({u, v, 1}).pos);
    }
    e1->data(ps1);

    EdgePtr e2 = std::make_shared<Edge>(edge2);
    e2->Id(id+"_edge2");
    e2->setExtend(ex2);
    vector<glm::vec3> ps2;
    for(int i =0; i<10; i++){
        float u = angle;
        float v = length/9*i;
        ps2.push_back(paraToWorld({u, v, 1}).pos);
    }
    e2->data(ps2);

    EdgePtr e3 = std::make_shared<Edge>(edge3);
    e3->Id(id+"_edge3");
    e3->setExtend(ex3);
    vector<glm::vec3> ps3;
    for(int i =0; i<10; i++){
        float u = angle/9*i;
        float v = 0;
        ps3.push_back(paraToWorld({u, v, 1}).pos);
    }
    e3->data(ps3);

    EdgePtr e4 = std::make_shared<Edge>(edge4);
    e4->Id(id+"_edge4");
    e4->setExtend(ex4);
    vector<glm::vec3> ps4;
    for(int i =0; i<10; i++){
        float u = angle/9*i;
        float v = length;
        ps4.push_back(paraToWorld({u, v, 1}).pos);
    }
    e4->data(ps4);
    return vector<EdgePtr>{e1, e2, e3, e4};
}


vector<glm::vec3> CylinderAssist::intersectionPoints(glm::vec3 worldPos, glm::vec3 worldDir){
    auto localPos = world3DToLocal(worldPos, "pos");
    auto localDir = world3DToLocal(worldDir, "dir");
    float dx = localDir.x;
    float dy = localDir.y;
    float px = localPos.x;
    float py = localPos.y;
    std::complex<float> A = dx*dx+dy*dy;
    std::complex<float> B = 2*(px*dx + py*dy);
    std::complex<float> C = px*px + py*py - R*R;
    if(A.real() == 0){
        return vector<glm::vec3>();
    }
    complex<float> x1 = (-1.0f*B - sqrt(B*B - 4.0f*A*C))/(2.0f*A);
    complex<float> x2 = (-1.0f*B + sqrt(B*B - 4.0f*A*C))/(2.0f*A);
    vector<float> xs;
    if(abs(x1.imag()) < 1e-2){
        xs.push_back(x1.real());
    }
    if(abs(x2.imag()) < 1e-2){
        xs.push_back(x2.real());
    }
    glm::mat4 invT = glm::inverse(T);
    vector<glm::vec3> points;
    for(auto i:xs){
        glm::vec3 p = worldPos + i*worldDir;
        auto localPos = world3DToLocal(p, "pos");
        float r = sqrt(localPos.x*localPos.x + localPos.y*localPos.y);
        if(abs(r-R) < 1e-2){
            vector<float> uv = local3DToUV(localPos);
            if(isInPara(uv)){
                glm::vec3 norm = outNorm(p);
                if(glm::dot(norm, worldDir) < 0){
                    points.push_back(p);
                }
            }
            else{}
        }
        else{}
    }
    return points;
}

bool CylinderAssist::isInPara(vector<float> uv){
    float pi = asin(1)*2;
    float u = uv.at(0);
    float v = uv.at(1);
    if(0<=v && v<=length){
        while(u<0){
            u = u+2*pi;
        }
        while(u>2*pi){
            u = u-2*pi;
        }
        if(u<=angle){
            return true;
        }
        else{
            return false;
        }
    }
    else{
        return false;
    }
}

Pos CylinderAssist::outNorm(Pos p){
    Pos local = world3DToLocal(p, "pos");
    auto uv = local3DProjectToUV(local);
    Dir uTan = localDir(uv.at(0), uv.at(1), 1, 0);
    Dir vTan = localDir(uv.at(0), uv.at(1), 0, 1);
    Dir norm = glm::cross(uTan, vTan);
    assert(abs(glm::length(norm) - 1) < 1e-2);
    return local3DToWorld(norm, "dir");
}
