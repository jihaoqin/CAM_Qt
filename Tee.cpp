#include "Tee.h"
#include "utility.h"
using namespace std;
Tee::Tee():modelMat(glm::mat4(1.0)), color(Color::RED)

{
    //TODO::
    //初始化lengthMain, lengthBranch
    pipeR = 100;
    sideR = 50;
    Mesh m = generateLeftRing();
    meshVec.push_back(m);
}

Mesh Tee::generateLeftRing(){
    vector<Vertex> edge;
    vector<Vertex> verVec;
    int COLUMN = 180;
    for(int i = 0; i < COLUMN+1; i++){
        Vertex v;
        float y = pipeR + sideR;
        float theta = -1*3.14/(COLUMN)*i;
        float z = pipeR * cos(theta);
        float x = pipeR * sin(theta);
        v.vertex = glm::vec3(x, y, z);
        v.normal = glm::vec3(x, 0, z);
        v.coordinate = glm::vec2(0);
        verVec.push_back(v);
        edge.push_back(v);
    }
    glm::vec3 anchor(-1*(pipeR+sideR), pipeR+sideR, 0);
    glm::vec3 dir(0, 0, 1);
    int ROW = 100;
    float alpha = -90;
    for (int i = 0; i< ROW+1; i++){
        float alpha_i = alpha*3.14/180/(ROW)*i;
        for (auto p:edge){
            Vertex pNew = utility::lineRotateVertex(anchor, dir, p, alpha_i);
            verVec.push_back(pNew);
        }
    }
    auto INDEX = [COLUMN](int i, int j)->int{return i+(COLUMN+1)*j;};
    vector<unsigned int> indexVec;
    for (int i =0; i<COLUMN - 1; i++){
        for(int j = 0; j< ROW - 1; j++){
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
