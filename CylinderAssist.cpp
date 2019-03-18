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
    float un,vn;
    while(v<0){
        v=v+2*pi;
    }
    while(v>2*pi){
        v=v-2*pi;
    }

    if(v<=0){
        if(u<=angle){
            un = u;
            vn = 0;
        }
        else {
            un = angle;
            v = 0;
        }
    }
    else if(0<v || v<length){
        if(u<=angle){
            un = u;
            vn = v;
        }
        else{
            un = angle;
            vn = v;
        }
    }
    else{
        if(u<=angle){
            un = u;
            vn = length;
        }
        else{
            un = angle;
            vn = length;
        }
    }

    while(un > pi){
        un = un -2*pi;
    }
    return vector<float>{un, vn};
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
    glm::normalize(c);
    glm::vec3 part = dir - glm::dot(dir,c)*c;
    float uAng = atan2(glm::dot(vTan, part), glm::dot(uTan, part));
    return CPPara{u, v, uAng};
}

glm::vec3 CylinderAssist::localDir(float u, float v, float uW, float vW){
    glm::vec3 uTan{-1*R*sin(u), R*cos(u), 0};
    glm::normalize(uTan);
    glm::vec3 vTan{0,0,1};
    return glm::normalize(uW*uTan + vW*vTan);
}

PosDir CylinderAssist::paraToWorld(CPPara a){
    float u = a.u;
    float v = a.v;
    float uAng = a.uAng;
    glm::vec3 pos{R*cos(u), R*sin(v), v};
    glm::vec3 dir = localDir(u, v, cos(uAng), sin(uAng));
    glm::vec3 worldPos = local3DToWorld(pos, "pos");
    glm::vec3 worldDir = local3DToWorld(dir, "dir");
    return PosDir{worldPos, worldDir};
}

vector<EdgePtr> CylinderAssist::getEdges(){
    float pi = asin(1)*2;
    auto edge1 = [pi](float u, float v)->bool{
        return u<0? true:false;
    };
    float aangle = angle;
    auto edge2 = [pi, aangle](float u, float v)->bool{
        return u>aangle? true:false;
    };
    auto edge3 = [](float u, float v)->bool{
        return v<0? true:false;
    };
    float l = length;
    auto edge4 = [l](float u, float v)->bool{
        return v>l? true:false;
    };
    EdgePtr e1 = std::make_shared<Edge>(edge1);
    e1->Id(id+"_edge1");
    EdgePtr e2 = std::make_shared<Edge>(edge2);
    e1->Id(id+"_edge2");
    EdgePtr e3 = std::make_shared<Edge>(edge3);
    e1->Id(id+"_edge3");
    EdgePtr e4 = std::make_shared<Edge>(edge4);
    e1->Id(id+"_edge4");
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
                points.push_back(p);
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
