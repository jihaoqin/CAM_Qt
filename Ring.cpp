#include "Ring.h"
#include <cmath>
#include <algorithm>
#include "numpy.h"
using namespace utility;
using namespace std;

Ring::Ring(double R_, double r_, double angle_, glm::vec3 anchor_, glm::vec3 zdir_, glm::vec3 xdir_)
    :R(R_), r(r_), angle(angle_), anchor(anchor_), zdir(zdir_), xdir(xdir_), m(){
    zdir = glm::normalize(zdir_);
    xdir = glm::normalize(xdir_);
    vector<Vertex> edge = generateEdge();
    m = utility::generateRevolution(anchor,zdir, edge, angle);
    box = m.boundingBox();
}

BoundingBox Ring::boundingBox(){
    return box;
}

void Ring::bindGL(QOpenGLContext *c){
    m.bindGL(c);
}

void Ring::draw(std::shared_ptr<GLProgram> p){
    m.draw();
}

vector<Vertex> Ring::generateEdge(){
    glm::vec3 ydir = glm::cross(zdir, xdir);
    glm::mat4 T;
    utility::setPos(T, anchor);
    utility::setXDir(T, xdir);
    utility::setYDir(T, ydir);
    utility::setZDir(T, zdir);
    vector<Vertex> edge;
    int COLUMN = 180;
    for(int i = 0; i < COLUMN+1; i++){
        double theta = utility::PI/COLUMN*i;
        Vertex v;
        glm::vec3 pos;
        glm::vec3 n;
        pos.x = R - r*sin(theta);
        pos.y = 0;
        pos.z = r*cos(theta);
        n = glm::normalize(pos - glm::vec3(R, 0, 0));
        v.vertex = T*glm::vec4(pos, 1);
        v.normal = RInMatrix(T)*n;
        v.coordinate = glm::vec2(0);
        edge.push_back(v);
    }
    return edge;
}

Ring::~Ring(){

}

vector<glm::vec3> Ring::intersectionPoints(glm::vec3 worldPos, glm::vec3 worldDir){

    glm::mat4 T = utility::createMat(anchor,zdir,glm::cross(zdir, xdir));
    glm::mat4 invT = glm::inverse(T);
    glm::mat3 invR = utility::RInMatrix(invT);
    glm::vec3 pos = utility::getPos(invT*glm::vec4(worldPos.x, worldPos.y, worldPos.z, 1.0));
    glm::vec3 dir = invR*worldDir;

    vector<double> realSolutions;


        auto coe = generateCoe(pos, dir);
        //Root solutions = utility::root4(coe);
        //vector<complex<double>> solVec{solutions.x1, solutions.x2, solutions.x3, solutions.x4};
        auto solVec = numpy::roots(coe);

        /*
    vector<complex<double>> solVec = utility::findRoot(coe);
    */
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
        /*
        double minReal = *std::min_element(realSolutions.begin(), realSolutions.end());
        glm::vec3 nearestPoint = pos + dir*(float)minReal;
        if(utility::length(pos - nearestPoint) < 1e-2){
            break;
        }
        else{
            glm::vec3 lastPos = pos;
            pos = pos + 0.33f*(nearestPoint - pos);
        }
        */

    //检查解是否在参数空间内
    vector<float> sol;
    vector<vector<float>> group;
    for(auto i:realSolutions){
        auto testPoint = pos+dir*(float)i;
        vector<float> para = paraWithPoint(testPoint);
        group.push_back(para);
        float theta = para.at(0);
        float alpha = para.at(1);
        if(theta<=angle && theta >=0){
            if((-1*PI<=alpha && alpha<=-0.5*PI)||( 0.5*PI<=alpha && alpha<PI)){
                sol.push_back(i);
            }
        }
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

vector<double> Ring::generateCoe(glm::vec3 pos, glm::vec3 dir){
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

vector<float> Ring::paraWithPoint(glm::vec3 pos){
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
    if(isSamePoint(theta_2, alpha_1, pos)){
        return group2;
    }
    if(isSamePoint(theta_3, alpha_2, pos)){
        return group3;
    }
    if(isSamePoint(theta_4, alpha_2, pos)){
        return group4;
    }

}

bool Ring::isSamePoint(float theta, float alpha, glm::vec3 pos){
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
