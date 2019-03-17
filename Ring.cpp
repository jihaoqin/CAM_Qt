#include "Ring.h"
#include <cmath>
#include <algorithm>
#include "RingAssist.h"
using namespace utility;
using namespace std;

Ring::Ring(double R_, double r_, double angle_, glm::vec3 anchor_, glm::vec3 zdir_, glm::vec3 xdir_)
    :R(R_), r(r_), angle(angle_), anchor(anchor_), zdir(zdir_), xdir(xdir_), m(),edges(){
    zdir = glm::normalize(zdir_);
    xdir = glm::normalize(xdir_);
    vector<Vertex> edge = generateEdge();
    m = utility::generateRevolution(anchor,zdir, edge, angle);
    box = m.boundingBox();
    RingAssist assist(*this);
    edges = assist.getEdges();
}

BoundingBox Ring::boundingBox(){
    return box;
}

void Ring::bindGL(QOpenGLContext *c){
    if(binded == false){
        m.bindGL(c);
    }
    binded = true;
}

void Ring::draw(std::shared_ptr<GLProgram> p){
    if(visiable == true){
        m.draw();
    }
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

vector<HalfPoint> Ring::intersectionPoints(glm::vec3 worldPos, glm::vec3 worldDir){
    RingAssist rA(*this);

    auto points = rA.intersectionPoints(worldPos, worldDir);
    vector<HalfPoint> result;
    std::string s(getId());
    for(auto i:points){
        result.push_back(HalfPoint{i, getId()});
    }
    return result;
}
