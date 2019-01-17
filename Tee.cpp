#include "Tee.h"
#include "utility.h"
using namespace std;
Tee::Tee()
{

}

Mesh Tee::generateLeftRing(){
    vector<Vertex> edge;
    vector<Vertex> verVec;
    int NUM = 180;
    for(int i = 0; i < NUM+1; i++){
        Vertex v;
        float y = pipeR + sideR;
        float theta = -1*3.14/NUM*i;
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
    NUM = 100;
    float alpha = -90;
    for (int i = 0; i< NUM+1; i++){
        float alpha_i = alpha*3.14/180/NUM*i;
        for (auto p:edge){
            Vertex pNew = utility::lineRotateVertex(anchor, dir, p, alpha_i);
            verVec.push_back(pNew);
        }
    }
}
