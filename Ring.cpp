#include "Ring.h"
#include "utility.h"

using namespace utility;
using namespace std;

Ring::Ring(double R_, double r_, double angle_, glm::vec3 anchor_, glm::vec3 zdir_, glm::vec3 xdir_)
    :R(R_), r(r_), angle(angle_), anchor(anchor_), zdir(zdir_), xdir(xdir_), m(){
    zdir = glm::normalize(zdir_);
    xdir = glm::normalize(xdir_);
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
    m = utility::generateRevolution(anchor,zdir, edge, angle);
}

BoundingBox Ring::boundingBox(){
    return m.boundingBox();
}

void Ring::bindGL(QOpenGLContext *c){
    m.bindGL(c);
}

void Ring::draw(std::shared_ptr<GLProgram> p){
    m.draw(p);
}
