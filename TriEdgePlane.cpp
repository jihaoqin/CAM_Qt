#include "TriEdgePlane.h"
#include "TriEdgePlaneAssist.h"
#include "utility.h"
#include "Tee.h"

using namespace utility;
TriEdgePlane::TriEdgePlane(){

}

TriEdgePlane::TriEdgePlane(QString s, float radius, glm::vec3 loc, glm::vec3 uDirection, glm::vec3 normDir)
    :R(radius), anchor(loc), uDir(glm::normalize(uDirection)), norm(glm::normalize(normDir)),edges()
{
    setId(s.toLatin1().data());
    TriEdgePlaneAssist assist(*this);
    edges = assist.getEdges();
    initialMesh();
}


void TriEdgePlane::initialMesh(){
    glm::vec3 vDir = glm::cross(norm, uDir);
    norm = glm::cross(uDir, vDir);
    glm::mat4 T;
    setPos(T,anchor);
    setXDir(T, uDir);
    setYDir(T, vDir);
    setZDir(T, norm);
    vector<vector<glm::vec3>> edgeVec;
    edgeVec.push_back(vector<glm::vec3>());
    edgeVec.push_back(vector<glm::vec3>());
    vector<glm::vec3> &edge_1 = edgeVec.at(0);
    vector<glm::vec3> &edge_2 = edgeVec.at(1);
    int NUM = 50;
    for (int i = 0; i<NUM +1;i++){
        using  utility::PI;
        float theta = PI/2*(-1)/NUM * i - PI/2;
        glm::vec3 p = R * glm::vec3{1,0,0}*cos(theta) + R *glm::vec3{0,1,0} * sin(theta)+ glm::vec3{R, R, 0};
        glm::vec3 worldP = T*glm::vec4(p, 1);
        glm::vec3 p2{p.x*(-1), p.y, p.z};
        glm::vec3 worldP2 = T*glm::vec4(p2, 1);
        edge_1.push_back(worldP);
        edge_2.push_back(worldP2);
    }
    reverse(edge_2.begin(), edge_2.end());

    vector<Vertex> verVec;
    for(int i = 0; i < edgeVec.size(); i++){
        vector<glm::vec3> &edge = edgeVec.at(i);
        for(int j = 0; j<edge.size() - 1; j++){
            Vertex v1;
            Vertex v2;
            Vertex v3;
            v1.vertex = anchor;
            v1.normal = norm;
            v1.coordinate = glm::vec2(0);

            v2.vertex = edge.at(j);
            v2.normal = norm;
            v2.coordinate = glm::vec2(0);

            v3.vertex = edge.at(j+1);
            v3.normal = norm;
            v3.coordinate = glm::vec2(0);

            verVec.push_back(v1);
            verVec.push_back(v2);
            verVec.push_back(v3);
        }
    }
    vector<unsigned int> indexVec;
    for(int i = 0; i<verVec.size(); i++){
        indexVec.push_back(i);
    }
    m = Mesh(verVec, indexVec);
}

void TriEdgePlane::bindGL(QOpenGLContext *c){
    if(binded == false){
        m.bindGL(c);
    }
    binded = true;
}

void TriEdgePlane::draw(std::shared_ptr<GLProgram> p){
    if(visiable == true){
        m.draw();
    }
}

void TriEdgePlane::initial(float radius, glm::vec3 loc, glm::vec3 uDirection, glm::vec3 normDir){
    R = radius;
    anchor = loc;
    uDir = uDirection;
    norm = normDir;
    initialMesh();
}

std::vector<HalfPoint> TriEdgePlane::intersectionPoints(glm::vec3 worldPos, glm::vec3 worldDir){
    TriEdgePlaneAssist assist(*this);
    auto points = assist.intersectionPoitns(worldPos, worldDir);
    vector<HalfPoint> result;
    std::string s(getId());
    for(auto i:points){
        result.push_back(HalfPoint{i, getId()});
    }
    return result;
}

vector<EdgePtr> TriEdgePlane::getEdges(){
    return edges;
}
