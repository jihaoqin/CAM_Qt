#include "RingAssist.h"
#include "utility.h"
#include <complex>
#include "numpy.h"
#include "Edge.h"
using namespace std;
RingAssist::RingAssist(Ring& ring)
{
    R = ring.R;
    r = ring.r;
    angle = ring.angle;//弧度制
    anchor = ring.anchor;
    zdir = ring.zdir;
    xdir = ring.xdir;
    id = ring.getId();
    T = utility::createMat(anchor,zdir,glm::cross(zdir, xdir));
}

vector<glm::vec3> RingAssist::intersectionPoints(glm::vec3 worldPos, glm::vec3 worldDir){
    glm::mat4 T = utility::createMat(anchor,zdir,glm::cross(zdir, xdir));
    glm::mat4 invT = glm::inverse(T);
    glm::mat3 invR = utility::RInMatrix(invT);
    glm::vec3 pos = utility::getPos(invT*glm::vec4(worldPos.x, worldPos.y, worldPos.z, 1.0));
    glm::vec3 dir = invR*worldDir;

    vector<double> realSolutions;
    auto coe = generateCoe(pos, dir);
    auto solVec = numpy::roots(coe);
    auto isReal = [](complex<double> num)->bool{
        return abs(num.imag())<0.000001? true:false;
    };
    vector<glm::vec3> allPoints;
    for(auto i:solVec){
        if(isReal(i)&&i.real()>0){
            realSolutions.push_back(i.real());
            allPoints.push_back(pos + dir*(float)i.real());
        }
    }
    if(realSolutions.empty()){
        return vector<glm::vec3>();
    }

    //检查解是否在参数空间内
    vector<float> sol;
    vector<vector<float>> group;
    for(auto i:realSolutions){
        auto testPoint = pos+dir*(float)i;
        vector<float> para = paraWithLocalPoint(testPoint);
        if(para.size()!=0){
            group.push_back(para);
            float theta = para.at(0);
            float alpha = para.at(1);
            if(inParaSpace(theta, alpha)){
                glm::vec3 norm = localNorm(theta, alpha);
                if(glm::dot(norm, dir) > 0){
                    sol.push_back(i);
                }
            }
            else{}
        }
        else{}
    }

    //输出解
    vector<glm::vec3> points;
    for(auto i:sol){
        auto p = pos + dir*(float)i;
        auto homo = glm::vec4(p,1.0);
        glm::vec3 worldP = T*glm::vec4(p, 1.0);
        points.push_back(worldP);
    }
    return points;
}

vector<double> RingAssist::generateCoe(glm::vec3 pos, glm::vec3 dir){
    double x0 = pos.x;
    double y0 = pos.y;
    double z0 = pos.z;

    double dx = dir.x;
    double dy = dir.y;
    double dz = dir.z;

    double A = pow(dx,2) + pow(dy, 2) + pow(dz, 2);
    double B = 2*(x0*dx+y0*dy+z0*dz);
    double C = pow(x0,2) + pow(y0, 2) + pow(z0, 2) - R*R -r*r;

    double coe1 = A*A;
    double coe2 = 2*A*B;
    double coe3 = B*B+2*A*C+4*R*R*dz*dz;
    double coe4 = 2*(B*C+4*R*R*z0*dz);
    double coe5 = C*C+4*R*R*z0*z0-4*R*R*r*r;

    vector<double> coe{coe1, coe2, coe3, coe4, coe5};
    return coe;
}

vector<float> RingAssist::paraWithLocalPoint(glm::vec3 pos){
    float x = pos.x; float y = pos.y; float z = pos.z;

    float alpha_1 = asin(z/r);
    float theta_1; float theta_2;
    float alpha_2;
    float theta_3; float theta_4;
    if(alpha_1 >0){
        alpha_2 = utility::PI - alpha_1;
    }
    else{
        alpha_2 = -1*utility::PI - alpha_1;
    }

    double costheta_12 = x /(R+r*cos(alpha_1));
    if(abs(costheta_12) > 1){
        //alpha_1是错的
        theta_1 = 0;
        theta_2 = 0;
    }
    else{
        theta_1 = acos(costheta_12);
        theta_2 = -1*theta_1;
    }

    double costheta_34 = x /(R+r*cos(alpha_2));
    if(abs(costheta_34) > 1){
        //alpha_2是错的
        theta_3 = 0;
        theta_4 = 0;
    }
    else{
        theta_3 = acos(costheta_34);
        theta_4 = -1*theta_3;
    }
    // 判断参数解{theta, alpha}是否和pos相符
    vector<float> group1{theta_1, alpha_1};
    vector<float> group2{theta_2, alpha_1};
    vector<float> group3{theta_3, alpha_2};
    vector<float> group4{theta_4, alpha_2};

    if(isSamePoint(theta_1, alpha_1, pos)){
        return group1;
    }
    else if(isSamePoint(theta_2, alpha_1, pos)){
        return group2;
    }
    else if(isSamePoint(theta_3, alpha_2, pos)){
        return group3;
    }
    else if(isSamePoint(theta_4, alpha_2, pos)){
        return group4;
    }
    else{
        return vector<float>();
    }

}

bool RingAssist::isSamePoint(float theta, float alpha, glm::vec3 pos){
    glm::vec3 posFromPara;
    posFromPara.x = R*cos(theta) + r*cos(alpha)*cos(theta);
    posFromPara.y = (R+r*cos(alpha)) * sin(theta);
    posFromPara.z = r*sin(alpha);
    float dist = utility::length(pos - posFromPara);
    if(dist < 0.01){
        return true;
    }
    else{
        return false;
    }
}

bool RingAssist::inParaSpace(float theta, float alpha){
    if(theta<=angle && theta >=0){
        double pi = utility::PI;
        if((-1*pi<=alpha && alpha<=-0.5*pi)||( 0.5*pi<=alpha && alpha<pi)){
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

glm::vec3 RingAssist::paraToLocal3D(float theta, float alpha){
    double x = R*cos(theta) + r*cos(alpha)*cos(theta);
    double y = (R+r*cos(alpha)) * sin(theta);
    double z = r*sin(alpha);
    return glm::vec3(x,y,z);
}

glm::vec3 RingAssist::localTangentDir(float theta, float alpha, double uWeight, double vWeight){
    glm::vec3 uDir;
    glm::vec3 vDir;
    vDir.x = -1*r*sin(alpha)*cos(theta);
    vDir.y = -1*r*sin(alpha)*sin(theta);
    vDir.z = r*cos(alpha);
    uDir.x = -1*(R+r*cos(alpha))*sin(theta);
    uDir.y = (R+r*cos(alpha))*cos(theta);
    uDir.z = 0;
    uDir = glm::normalize(uDir);
    vDir = glm::normalize(vDir);
    glm::vec3 dir = (float)uWeight*uDir + (float)vWeight*vDir;
    dir = glm::normalize(dir);
    return dir;
}

glm::vec3 RingAssist::local3DToWorld(glm::vec3 local, const char* flag){
    glm::mat4 T = utility::createMat(anchor,zdir,glm::cross(zdir, xdir));
    float four;
    if(QString("pos").contains(flag)){
        four = 1;
    }
    else if(QString("dir").contains(flag)){
        four = 0;
    }
    else{
        assert(1==0);
    }
    glm::vec3 world = T*glm::vec4(local, four);
    return world;
}
glm::vec3 RingAssist::world3DToLocal(glm::vec3 worldPos, const char* flag){
    glm::mat4 T = utility::createMat(anchor,zdir,glm::cross(zdir, xdir));
    glm::mat4 invT = glm::inverse(T);
    float four;
    if(QString("pos").contains(flag)){
        four = 1;
    }
    else if(QString("dir").contains(flag)){
        four = 0;
    }
    else{
        assert(1==0);
    }
    glm::vec3 localPos = invT*glm::vec4(worldPos, four);
    return localPos;
}

glm::vec3 RingAssist::worldDir(CPPara para){
    auto localDir = localTangentDir(para.u, para.v, cos(para.uAng), sin(para.uAng));
    auto world = local3DToWorld(localDir,"dir");
    return world;
}

PosDir RingAssist::localProject(glm::vec3 pos, glm::vec3 dir){
    CPPara p = local3DProjectToPara(pos, dir);
    glm::vec3 nearPos = paraToLocal3D(p.u, p.v);
    glm::vec3 nearDir = localTangentDir(p.u, p.v, cos(p.uAng),sin(p.uAng));
    return PosDir{nearPos,nearDir};
}


CPPara RingAssist::local3DProjectToPara(glm::vec3 pos, glm::vec3 dir){
    auto uv = local3DProjectToUV(pos);
    float nearTheta = uv.at(0);
    float nearAlpha = uv.at(1);
    CPPara p;
    p.u = nearTheta;
    p.v = nearAlpha;

    glm::vec3 thetaDir = localTangentDir(nearTheta, nearAlpha, 1,0);
    glm::vec3 alphaDir = localTangentDir(nearTheta, nearAlpha, 0,1);
    glm::vec3 norm = glm::cross(thetaDir, alphaDir);
    assert(abs(glm::length(norm) - 1) < 1e-2);
    glm::vec3 nearDir = dir - (dir*norm)*norm;
    float tany = glm::dot(nearDir, alphaDir);
    float tanx = glm::dot(nearDir, thetaDir);
    float uAngle = atan2(tany, tanx);
    p.uAng = uAngle;
    return p;
}

PosDir RingAssist::worldProject(glm::vec3 pos, glm::vec3 dir){
    glm::vec3 localPos = world3DToLocal(pos, "pos");
    glm::vec3 localDir = world3DToLocal(dir, "dir");
    PosDir p1 = localProject(localPos, localDir);
    glm::vec3 worldPos = local3DToWorld(p1.pos, "pos");
    glm::vec3 worldDir = local3DToWorld(p1.dir, "dir");
    return PosDir{worldPos, worldDir};
}

vector<float> RingAssist::local3DProjectToUV(glm::vec3 pos){
    float x = pos.x; float y = pos.y; float z = pos.z;
    float theta = atan2(y, x);
    float alpha = atan2(z, sqrt(x*x + y*y) - R);
    float nearTheta;
    float nearAlpha;
    if(inParaSpace(theta, alpha)){
        nearTheta = theta;
        nearAlpha = alpha;
    }
    else{
        //找到最近的点
        double pi = utility::PI;
        if(theta<0){
            if((alpha >= 0.5*pi && alpha <= pi) || (alpha <= -0.5*pi && alpha >= -1*pi)){
                nearTheta = 0;
                nearAlpha = alpha;
            }
            else if(alpha>-0.5*pi && alpha<0){
                nearTheta = 0;
                nearAlpha = -0.5*pi;
            }
            else if(alpha >=0 && alpha <0.5*pi){
                nearTheta = 0;
                nearAlpha = 0.5*pi;
            }
            else{
                assert(1 == 0);
            }
        }
        else if(theta >=0 && theta <= angle){
            if((alpha <=pi && alpha >= 0.5*pi) || (alpha >= -1*pi && alpha <= -0.5*pi)){
                nearTheta = theta;
                nearAlpha = alpha;
            }
            else if(alpha >=0 && alpha < 0.5*pi){
                nearTheta = theta;
                nearAlpha = 0.5*pi;
            }
            else if(alpha <0 && alpha > -0.5*pi){
                nearTheta = theta;
                nearAlpha = -0.5*pi;
            }
            else{
                assert(1 == 0);
            }
        }
        else if(theta > angle){
            if((alpha >= 0.5*pi && alpha <= pi) || (alpha <= -0.5*pi && alpha >= -1*pi)){
                nearTheta = angle;
                nearAlpha = alpha;
            }
            else if(alpha>-0.5*pi && alpha<0){
                nearTheta = angle;
                nearAlpha = -0.5*pi;
            }
            else if(alpha >=0 && alpha <0.5*pi){
                nearTheta = angle;
                nearAlpha = 0.5*pi;
            }
            else{
                assert(1 == 0);
            }
        }
    }
    return vector<float>{nearTheta, nearAlpha};
}

PosDir RingAssist::CPParaToLocal(CPPara p){
    glm::vec3 pos = paraToLocal3D(p.u, p.v);
    glm::vec3 dir = localTangentDir(p.u, p.v, cos(p.uAng), sin(p.uAng));
    return PosDir{pos, dir};
}

glm::vec3 RingAssist::localNorm(float u, float v){
    glm::vec3 tanU = localTangentDir(u, v, 1, 0);
    glm::vec3 tanV = localTangentDir(u, v, 0, 1);
    glm::vec3 norm = glm::cross(tanU, tanV);
    return glm::normalize(norm);
}

vector<EdgePtr> RingAssist::getEdges(){
    float pi = asin(1)*2;
    float aangle = angle;
    auto uZeroEdge = [pi, aangle](float u, float v)->bool{
        while(u>pi+0.5*aangle){
            u = u - 2*pi;
        }
        while(u<0.5*aangle - pi){
            u = u + 2*pi;
        }
        return u<0? true:false;
    };
    auto uAngleEdge = [pi, aangle](float u, float v)->bool {
        while(u>pi+0.5*aangle){
            u = u - 2*pi;
        }
        while(u<0.5*aangle - pi){
            u = u + 2*pi;
        }
        return u> aangle? true:false;
    };
    auto vEdge1 = [pi](float u, float v)->bool{
        while(v<0.5*pi){
            v = v+2*pi;
        }
        while(v>2*pi){
            v= v- 2*pi;
        }
        return v > 1.5*pi? true:false;
    };
    auto vEdge2 = [pi](float u, float v)->bool{
        while(v<0.5*pi){
            v = v+2*pi;
        }
        while(v>2*pi){
            v= v- 2*pi;
        }
        return v < 0.5*pi? true:false;
    };

    auto e1 = std::make_shared<Edge>(uZeroEdge);
    e1->Id(id+"_edge1");
    vector<glm::vec3> ps1;
    for(int i =0; i<10; i++){
        float u = 0;
        float v = pi/2+pi/9*i;
        glm::vec3 localPos = paraToLocal3D(u, v);
        ps1.push_back(local3DToWorld(localPos, "pos"));
    }
    e1->data(ps1);

    auto e2 = std::make_shared<Edge>(uAngleEdge);
    e2->Id(id+"_edge2");
    vector<glm::vec3> ps2;
    for(int i =0; i<10; i++){
        float u = angle;
        float v = pi/2+pi/9*i;
        glm::vec3 localPos = paraToLocal3D(u, v);
        ps2.push_back(local3DToWorld(localPos, "pos"));
    }
    e2->data(ps2);

    auto e3 = std::make_shared<Edge>(vEdge1);
    e3->Id(id+"_edge3");
    vector<glm::vec3> ps3;
    for(int i =0; i<10; i++){
        float u = angle/9*i;
        float v = 1.5*pi;
        glm::vec3 localPos = paraToLocal3D(u, v);
        ps3.push_back(local3DToWorld(localPos, "pos"));
    }
    e3->data(ps3);

    auto e4 = std::make_shared<Edge>(vEdge2);
    e4->Id(id+"_edge4");
    vector<glm::vec3> ps4;
    for(int i =0; i<10; i++){
        float u = angle/9*i;
        float v = 0.5*pi;
        glm::vec3 localPos = paraToLocal3D(u, v);
        ps4.push_back(local3DToWorld(localPos, "pos"));
    }
    e4->data(ps4);
    vector<EdgePtr> edges;
    edges.push_back(e1);
    edges.push_back(e2);
    edges.push_back(e3);
    edges.push_back(e4);
    return edges;
}
