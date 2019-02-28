#include "Ring.h"
#include <cmath>
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

vector<glm::vec3> Ring::intersectionPoints(glm::vec3 begin, glm::vec3 dir){
    auto coe = generateCoe(begin, dir);
    Root solutions = utility::root4(coe);
    vector<double> realSolutions;
    auto isReal = [](complex<double> num)->bool{
        return abs(num.imag())<0.000001? true:false;
    };
    if(isReal(solutions.x1)){
        realSolutions.push_back(solutions.x1.real());
    }
    if(isReal(solutions.x2)){
        realSolutions.push_back(solutions.x2.real());
    }
    if(isReal(solutions.x3)){
        realSolutions.push_back(solutions.x3.real());
    }
    if(isReal(solutions.x4)){
        realSolutions.push_back(solutions.x4.real());
    }

    //检查解是否在参数空间内
    vector<double> sol;
    for(auto i:realSolutions){
        vector<float> para = paraWithPoint(begin + dir*(float)i);
        sol.push_back(i);
    }

    //输出解
    vector<glm::vec3> points;
    for(auto i:sol){
        auto p = begin + dir*(float)i;
        points.push_back(p);
    }
    return points;
}

vector<double> Ring::generateCoe(glm::vec3 worldPos, glm::vec3 worldDir){
    glm::mat4 T = utility::createMat(anchor,zdir,glm::cross(zdir, xdir));
    glm::mat4 invT = glm::inverse(T);
    glm::mat3 invR = utility::RInMatrix(invT);
    glm::vec3 pos = invT*glm::vec4(worldPos.x, worldPos.y, worldPos.z, 1.0);
    glm::vec3 dir = invR*worldDir;
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
        alpha_2 = alpha_1 - utility::PI;
    }
    else{
        alpha_2 = alpha_1 + utility::PI;
    }

    double costheta_12 = x /(R+r*cos(alpha_1));
    if(abs(costheta_12) > 1){
        //alpha_1是错的
        theta_1 = 0;
        theta_2 = 0;
    }
    else{
        theta_1 = acos(costheta_12);
        theta_2 = theta_1 - utility::PI;
    }

    double costheta_34 = x /(R+r*cos(alpha_2));
    if(abs(costheta_34) > 1){
        //alpha_2是错的
        theta_3 = 0;
        theta_4 = 0;
    }
    else{
        theta_3 = acos(costheta_34);
        theta_4 = theta_3 - utility::PI;
    }
    // 判断参数解{theta, alpha}是否和pos相符
    vector<float> group1{theta_1, alpha_1};
    vector<float> group2{theta_2, alpha_1};
    vector<float> group3{theta_3, alpha_2};
    vector<float> group4{theta_4, alpha_2};

    vector<vector<float>> group;
    if(isSamePoint(theta_1, alpha_1, pos)){
        group.push_back(group1);
    }
    if(isSamePoint(theta_2, alpha_1, pos)){
        group.push_back(group2);
    }
    if(isSamePoint(theta_3, alpha_2, pos)){
        group.push_back(group3);
    }
    if(isSamePoint(theta_4, alpha_2, pos)){
        group.push_back(group4);
    }

    for(auto i:group){
        float theta = i.at(0);
        float alpha = i.at(1);
        if(theta<=angle && theta >=0){
            if((-1*PI<=alpha && alpha<=-0.5*PI)||( 0.5*PI<=alpha && alpha<PI)){
                return i;
            }
        }
    }
}

bool Ring::isSamePoint(float theta, float alpha, glm::vec3 pos){
    glm::vec3 posFromPara;
    posFromPara.x = R*cos(theta) + r*cos(alpha)*cos(theta);
    posFromPara.y = (R+r*cos(alpha)) * sin(theta);
    posFromPara.z = r*sin(alpha);
    float dist = utility::length(pos - posFromPara);
    if(dist < 0.0001){
        return true;
    }
    else{
        return false;
    }
}
