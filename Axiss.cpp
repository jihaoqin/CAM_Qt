#include "Axiss.h"
#include "PosDir.h"
Axiss::Axiss(float xmin, float xmax, float ymin, float ymax)
    :m_xmin(xmin), m_xmax(xmax), m_ymin(ymin), m_ymax(ymax),binded(false)
{
    initial();
}

void Axiss::bindGL(QOpenGLContext *c){
    if(binded == true){
        return;
    }

    core = c->versionFunctions<QOpenGLFunctions_4_3_Core>();
    core->glGenVertexArrays(1, &VAO);
    core->glGenBuffers(1,&VBO);
    core->glGenBuffers(1, &EBO);
    xEnd.bindGL(c);
    yEnd.bindGL(c);
    binded = true;

    core->glBindVertexArray(VAO);
    core->glBindBuffer(GL_ARRAY_BUFFER, VBO);
    core->glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vertexVec.size(), &(vertexVec[0]), GL_STATIC_DRAW);
    core->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    core->glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*indexVec.size(), &(indexVec[0]), GL_STATIC_DRAW);

    core->glEnableVertexAttribArray(0);
    core->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
    core->glEnableVertexAttribArray(1);
    core->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(3*sizeof(float)));
    core->glEnableVertexAttribArray(2);
    core->glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(6*sizeof(float)));
}

void Axiss::initial(){
    PosVec poss{
        {m_xmin, 0, 0},
        {m_xmax, 0, 0},
        {0, m_ymin, 0},
        {0, m_ymax, 0}
    };
    for(int i = 0; i < 4; i++){
        Vertex v;
        v.vertex = poss.at(i);
        v.normal = Dir{0,0,1};
        v.coordinate = glm::vec2{0,0};
        vertexVec.push_back(v);
    }

    float pi = asin(1)*2;
    PosVec xEndPoss ={
        {m_xmax, 0, 0},
        {m_xmax - m_r, cos()},
        {},
        {}
    };
    VertexVec xEndVertexs;
    for(int i = 0; i < 4; i++){

    }
}
