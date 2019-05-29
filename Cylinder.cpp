#include "Cylinder.h"
#include "utility.h"
#include "CylinderAssist.h"

Cylinder::Cylinder(QString s, glm::vec3 begin_, glm::vec3 end_, double R_, float angle_, glm::vec3 xDir_)
    :DataObject(), anchor(begin_), R(R_), angle(angle_)
{
    setId(s.toLatin1().data());
    xDir = glm::normalize(xDir_);
    zDir = glm::normalize(end_ - begin_);
    length = glm::length(end_ - begin_);
    CylinderAssist assist(*this);
    edges = assist.getEdges();
    generateCyliner();
}

Mesh& Cylinder::generateCyliner(){
    glm::vec3 end_1 = anchor;
    glm::vec3 end_2 = anchor+length*zDir;
    vector<Vertex> edge;
    int COLUMN = 2;
    glm::vec3 dir = end_2 - end_1;
    glm::vec3 radiusVec = xDir * R;
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
    m = utility::generateRevolution(end_1, end_2 - end_1, edge, angle);
    return m;
}



void Cylinder::bindGL(QOpenGLContext* c){
    if(binded == false){
        m.bindGL(c);
    }
    binded = true;
}
void Cylinder::draw(std::shared_ptr<GLProgram> program){
    if(visiable == true){
        //program->setVec3("material.color", {1.0f*76.0/350, 1.0f*180.0/350, 1.0f*231.0/350});
        m.draw();
    }
}

BoundingBox Cylinder::boundingBox(){
    return m.boundingBox();
}

Cylinder::~Cylinder(){

}

vector<HalfPoint> Cylinder::intersectionPoints(glm::vec3 worldPos, glm::vec3 worldDir){
    CylinderAssist rC(*this);

    auto points = rC.intersectionPoints(worldPos, worldDir);
    vector<HalfPoint> result;
    std::string s(getId());
    for(auto i:points){
        result.push_back(HalfPoint{i, getId()});
    }
    return result;
}

vector<EdgePtr> Cylinder::getEdges(){
    return edges;
}
