#include "Tee.h"
#include <algorithm>
#include "config.h"
using namespace std;
Tee::Tee(float _lengthMain, float _lengthBranch, float _pipeR, float _sideR):modelMat(glm::mat4(1.0)), color(Color::RED),
        lengthMain(_lengthMain), lengthBranch(_lengthBranch), pipeR(_pipeR), sideR(_sideR)

{
    setId("tee");
    Mesh up = generateCircle(glm::vec3(0, lengthBranch, 0), glm::vec3(0, 1, 0), pipeR);
    Mesh left = generateCircle(glm::vec3(-1.0*lengthMain/2, 0, 0), glm::vec3(-1, 0, 0),pipeR);
    Mesh right = generateCircle(glm::vec3(1.0*lengthMain/2, 0, 0), glm::vec3(1, 0, 0),pipeR);
    Mesh front = generateFrontPlane();
    Mesh back = generateBackPlane();
    Ring ringLeft(sideR+pipeR, pipeR, utility::PI/2,
                    glm::vec3(-(pipeR+sideR), pipeR+sideR,0), glm::vec3(0,0,-1), glm::vec3(1,0,0));
    Ring ringRight(sideR+pipeR, pipeR, utility::PI/2,
                    glm::vec3(pipeR+sideR, pipeR+sideR,0), glm::vec3(0,0,1), glm::vec3(-1,0,0));
    ringVec.push_back(ringLeft);
    ringVec.push_back(ringRight);
    Cylinder branch(glm::vec3(0, pipeR + sideR, 0), glm::vec3(0, lengthBranch, 0), pipeR);
    Cylinder mainPipe(glm::vec3(-lengthMain/2, 0, 0), glm::vec3(lengthMain/2, 0, 0), pipeR);
    cylinderVec.push_back(branch);
    cylinderVec.push_back(mainPipe);
    meshVec.push_back(up);
    meshVec.push_back(left);
    meshVec.push_back(right);
    meshVec.push_back(front);
    meshVec.push_back(back);
    //包围盒
    vector<BoundingBox> boxVec;
    for (auto mesh:meshVec){
        boxVec.push_back(mesh.boundingBox());
    }
    for (auto r:ringVec){
        boxVec.push_back(r.boundingBox());
    }
    for (auto c:cylinderVec){
        boxVec.push_back(c.boundingBox());
    }
    box = BoundingBox::OrBox(boxVec);
}

Mesh Tee::generateLeftRing(){
    glm::vec3 leftAnchor(-1*(pipeR+sideR), pipeR+sideR, 0);
    glm::vec3 leftDir(0, 0, 1);
    float leftAngle = -1*utility::PI/2;
    vector<Vertex> edge;
    int COLUMN = 180;
    for(int i = 0; i < COLUMN+1; i++){
        Vertex v;
        float y = pipeR + sideR;
        float theta = -1*utility::PI/(COLUMN)*i;
        float z = pipeR * cos(theta);
        float x = pipeR * sin(theta);
        v.vertex = glm::vec3(x, y, z);
        v.normal = glm::vec3(x, 0, z);
        v.coordinate = glm::vec2(0);
        edge.push_back(v);
    }
    Mesh m = generateRevolution(leftAnchor, leftDir, edge, leftAngle);
    return m;
}

Mesh Tee::generateRightRing(){
    glm::vec3 rightAnchor((pipeR+sideR), pipeR+sideR, 0);
    glm::vec3 rightDir(0, 0, 1);
    float rightAngle = utility::PI/2;
    vector<Vertex> edge;
    int COLUMN = 180;
    for(int i = 0; i < COLUMN+1; i++){
        Vertex v;
        float y = pipeR + sideR;
        float theta = utility::PI/(COLUMN)*i;
        float z = pipeR * cos(theta);
        float x = pipeR * sin(theta);
        v.vertex = glm::vec3(x, y, z);
        v.normal = glm::vec3(x, 0, z);
        v.coordinate = glm::vec2(0);
        edge.push_back(v);
    }
    Mesh m = generateRevolution(rightAnchor, rightDir, edge, rightAngle);
    return m;
}
void Tee::bindGL(QOpenGLContext * c){
    if(true == binded){
        qDebug()<<"Tee has been binded.\n";
        return;
    }
    for(unsigned int i = 0; i < meshVec.size(); i++){
       meshVec.at(i).bindGL(c);
    }
    for(unsigned int i = 0; i < ringVec.size(); i++){
        ringVec.at(i).bindGL(c);
    }
    for(unsigned int i = 0; i < cylinderVec.size(); i++){
        cylinderVec.at(i).bindGL(c);
    }
    binded = true;
}

void Tee::draw(std::shared_ptr<GLProgram> p){
    p->setMat4("model", modelMat);
    p->setVec3("material.color", color.rgb);
    if(visiable == false){
        return;
    }
    for(unsigned int i = 0; i < meshVec.size(); i++){
       meshVec.at(i).draw();
    }
    for(unsigned int i = 0; i < ringVec.size(); i++){
        ringVec.at(i).draw(p);
    }
    for(unsigned int i = 0; i < cylinderVec.size(); i++){
        cylinderVec.at(i).draw(p);
    }
}

Mesh Tee::generateRevolution(glm::vec3 anchor, glm::vec3 dir, std::vector<Vertex> edge, float angle){
    vector<Vertex> verVec;
    int COLUMN = edge.size()-1;
    int ROW = 100;
    for (int i = 0; i< ROW+1; i++){
        float alpha_i = angle/ROW*i;
        for (auto p:edge){
            Vertex pNew = utility::lineRotateVertex(anchor, dir, p, alpha_i);
            verVec.push_back(pNew);
        }
    }
    auto INDEX = [COLUMN](int i, int j)->int{return i+(COLUMN+1)*j;};
    vector<unsigned int> indexVec;
    for (int i =0; i<COLUMN; i++){
        for(int j = 0; j< ROW; j++){
            //可能是normal出现了问题
            indexVec.push_back(INDEX(i,j));
            indexVec.push_back(INDEX(i+1, j));
            indexVec.push_back(INDEX(i, j+1));

            indexVec.push_back(INDEX(i+1, j));
            indexVec.push_back(INDEX(i+1, j+1));
            indexVec.push_back(INDEX(i, j+1));
        }
    }
    //可以优化成return Mesh(verVec, indexVec);
    Mesh m(verVec, indexVec);
    return m;
}

Mesh Tee::generateCylinder(glm::vec3 end_1, glm::vec3 end_2, float r){
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
    Mesh m = generateRevolution(end_1, end_2 - end_1, edge, 2*utility::PI);
    return m;
}

Mesh Tee::generateBranchPipe(){
    glm::vec3 end_1(0, pipeR + sideR, 0);
    glm::vec3 end_2 = glm::vec3(end_1.x, lengthBranch, end_1.z);
    Mesh m = generateCylinder(end_1, end_2, pipeR);
    return m;
}

Mesh Tee::generateMainPipe(){
    float sideLength = lengthMain/2;
    glm::vec3 end_1(-1*sideLength, 0, 0);
    glm::vec3 end_2(sideLength, 0, 0);
    Mesh m = generateCylinder(end_1, end_2, pipeR);
    return m;
}

Mesh Tee::generateCircle(glm::vec3 anchor, glm::vec3 dir, float r){
    glm::vec3 a;
    if(!utility::isParallel(dir, glm::vec3(dir.y, -1*dir.x, dir.z))){
       a = glm::vec3(dir.y, -1*dir.x, dir.z);
    }
    if(!utility::isParallel(dir, glm::vec3(dir.z, dir.y, -1*dir.x))){
       a = glm::vec3(dir.z, dir.y, -1*dir.x);
    }
    glm::vec3 rand = dir + a;
    glm::vec3 radiusVec_1 = glm::normalize(glm::cross(dir, rand)) * r;
    glm::vec3 radiusVec_2 = glm::normalize(glm::cross(dir, radiusVec_1)) * r;

    Vertex point;
    point.vertex = anchor;
    point.normal = dir;
    point.coordinate = glm::vec2(0);
    vector<Vertex> verVec;
    verVec.push_back(point);
    int NUM = 180;
    for (int i = 0; i<NUM+1; i++){
        Vertex p;
        float theta = 2*utility::PI/NUM*i;
        auto t = anchor + radiusVec_1*cos(theta) + radiusVec_2 * sin(theta);
        p.vertex = t;
        p.normal = dir;
        p.coordinate = glm::vec2(0);
        verVec.push_back(p);
    }
    vector<unsigned int> indexVec;
    for (int i = 1; i<NUM+1; i++){
        indexVec.push_back(0);
        indexVec.push_back(i);
        indexVec.push_back(i+1);
    }
    Mesh m(verVec, indexVec);
    return m;
}

Mesh Tee::generatePlane(glm::vec3 anchor, vector<vector<glm::vec3>> edgeVec){
    vector<Vertex> verVec;
    for(int i = 0; i < edgeVec.size() - 1; i++){
        vector<glm::vec3> &edge = edgeVec.at(i);
        for(int j = 0; j<edge.size() - 1; j++){
            Vertex v1;
            Vertex v2;
            Vertex v3;
            v1.vertex = anchor;
            glm::vec3 normal = glm::normalize(glm::cross(edge.at(j) - anchor, edge.at(j+1) - anchor));
            v1.normal = normal;
            v1.coordinate = glm::vec2(0);

            v2.vertex = edge.at(j);
            v2.normal = normal;
            v2.coordinate = glm::vec2(0);

            v3.vertex = edge.at(j+1);
            v3.normal = normal;
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
    return Mesh(verVec, indexVec);
}

Mesh Tee::generateFrontPlane(){
    glm::vec3 anchor(0, 0, pipeR);
    vector<vector<glm::vec3>> edgeVec;
    edgeVec.push_back(vector<glm::vec3>());
    edgeVec.push_back(vector<glm::vec3>());
    edgeVec.push_back(vector<glm::vec3>());
    vector<glm::vec3> &edge_1 = edgeVec.at(0);
    vector<glm::vec3> &edge_2 = edgeVec.at(1);
    vector<glm::vec3> &edge_3 = edgeVec.at(2);
    int NUM = 50;
    for (int i = 0; i<NUM +1;i++){
        float theta = utility::PI/2*(-1)/NUM * i;
        glm::vec3 p =glm::vec3(sideR + pipeR,0,0) * cos(theta) + glm::vec3(0,sideR + pipeR,0) * sin(theta) + glm::vec3(-1*(pipeR + sideR), pipeR + sideR,anchor.z);
        edge_1.push_back(p);
        edge_2.push_back(glm::vec3(p.x*(-1), p.y, p.z));
    }
    reverse(edge_2.begin(), edge_2.end());
    edge_3.push_back(edge_1.back());
    edge_3.push_back(edge_2.front());
    return generatePlane(anchor, edgeVec);
}

Mesh Tee::generateBackPlane(){
    glm::vec3 anchor(0, 0, -1*pipeR);
    vector<vector<glm::vec3>> edgeVec;
    edgeVec.push_back(vector<glm::vec3>());
    edgeVec.push_back(vector<glm::vec3>());
    edgeVec.push_back(vector<glm::vec3>());
    vector<glm::vec3> &edge_1 = edgeVec.at(0);
    vector<glm::vec3> &edge_2 = edgeVec.at(1);
    vector<glm::vec3> &edge_3 = edgeVec.at(2);
    int NUM = 50;
    for (int i = 0; i<NUM +1;i++){
        float theta = utility::PI/2*(-1)/NUM * i;
        glm::vec3 p =glm::vec3(sideR + pipeR,0,0) * cos(theta) + glm::vec3(0,sideR + pipeR,0) * sin(theta) + glm::vec3(-1*(pipeR + sideR), pipeR + sideR,anchor.z);
        edge_1.push_back(p);
        edge_2.push_back(glm::vec3(p.x*(-1), p.y, p.z));
    }
    reverse(edge_1.begin(), edge_1.end());
    edge_3.push_back(edge_2.back());
    edge_3.push_back(edge_1.front());
    return generatePlane(anchor, edgeVec);
}

BoundingBox Tee::boundingBox(){
    return box;
}

Tee::~Tee(){

}

vector<glm::vec3> Tee::intersectionPoints(glm::vec3 pos, glm::vec3 dir){
    vector<glm::vec3> result;
    vector<glm::vec3> ringPoints;
    vector<glm::vec3> cylinderPoints;
    vector<glm::vec3> planePoints;
    for(auto r:ringVec){
        ringPoints = r.intersectionPoints(pos, dir);
        for(auto p:ringPoints){
            result.push_back(p);
        }
    }
    /*
    for(auto c:cylinderVec){
        cylinderPoints = c.intersectionPoints(pos, dir);
        for(auto p:cylinderPoints){
            result.push_back(p);
        }
    }
    for(auto p:planeVec){
        planePoints = p.intersectionPoints(pos, dir);
        for(auto i:planePoints){
            result.push_back(i);
        }
    }
    */
    return result;
}
