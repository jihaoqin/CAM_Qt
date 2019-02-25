#include "Cylinder.h"
#include "utility.h"

Cylinder::Cylinder(glm::vec3 begin_, glm::vec3 end_, double R_)
    :GLBinder(), begin(begin_), end(end_), R(R_)
{
    generateCyliner(begin, end, R);
}

Mesh& Cylinder::generateCyliner(glm::vec3 end_1, glm::vec3 end_2, float r){
    vector<Vertex> edge;
    int COLUMN = 180;
    glm::vec3 dir = end_2 - end_1;
    glm::vec3 a;
    if(!utility::isParallel(dir, glm::vec3(dir.y, -1*dir.x, dir.z))){
       a = glm::vec3(dir.y, -1*dir.x, dir.z);
    }
    if(!utility::isParallel(dir, glm::vec3(dir.z, dir.y, -1*dir.x))){
       a = glm::vec3(dir.z, dir.y, -1*dir.x);
    }
    glm::vec3 rand = dir + a;
    auto c = glm::cross(dir, rand);
    glm::vec3 radiusVec = glm::normalize(glm::cross(dir, rand)) * r;
    glm::vec3 end_1_new = end_1 + radiusVec;
    for(int i = 0; i< COLUMN+1; i++){
        Vertex v;
        glm::vec3 p = end_1_new + dir*(float)(1.0*i/COLUMN);
        glm::vec3 norm = glm::normalize(radiusVec);
        v.vertex = p;
        v.normal = norm;
        v.coordinate = glm::vec2(0);
        edge.push_back(v);
    }
    m = utility::generateRevolution(end_1, end_2 - end_1, edge, 2*utility::PI);
    return m;
}



void Cylinder::bindGL(QOpenGLContext* c){
    m.bindGL(c);
}
void Cylinder::draw(std::shared_ptr<GLProgram> program){
    m.draw(program);
}
