#include "Tee.h"
#include "utility.h"
using namespace std;
Tee::Tee():modelMat(glm::mat4(1.0)), color(Color::RED)

{
    //TODO::
    //初始化lengthMain, lengthBranch
    //rightRing
    pipeR = 100;
    sideR = 50;
    lengthBranch = 150;

    Mesh m = generateLeftRing();
    Mesh mR = generateRightRing();
    Mesh mBranch = generateBranchPipe();
    meshVec.push_back(m);
    meshVec.push_back(mR);
    meshVec.push_back(mBranch);
    vector<BoundingBox> boxVec;
    for (auto mesh:meshVec){
        boxVec.push_back(mesh.boundingBox());
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
    for(unsigned int i = 0; i < meshVec.size(); i++){
       meshVec.at(i).bindGL(c);
    }
    binded = true;
}

void Tee::draw(std::shared_ptr<GLProgram> program){
    program->setMat4("model", modelMat);
    program->setVec3("material.color", color.rgb);
    for(unsigned int i = 0; i < meshVec.size(); i++){
       meshVec.at(i).draw(program);
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
    glm::vec3 rand = dir + glm::vec3(10, 0, 0);
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
    glm::vec3 end_2 = glm::vec3(end_1.x, end_1.y+lengthBranch, end_1.z);
    Mesh m = generateCylinder(end_1, end_2, pipeR);
    return m;
}
