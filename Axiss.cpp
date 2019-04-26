#include "Axiss.h"
#include "PosDir.h"
#include "Color.h"
Axiss::Axiss(float xmin, float xmax, float ymin, float ymax, float r)
    :m_xmin(xmin), m_xmax(xmax), m_ymin(ymin), m_ymax(ymax), m_r(r), binded(false)
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

    xEnd.bindGL(c);
    yEnd.bindGL(c);
    binded = true;
}

void Axiss::draw(std::shared_ptr<GLProgram> p){
    if(binded == true){
        core->glBindVertexArray(VAO);
        core->glBindBuffer(GL_ARRAY_BUFFER, VBO);
        core->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        p->setMat4("model", glm::mat4(1.0f));
        p->setVec3("material.color", Color::RED);
        core->glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, (void*)(0));
        p->setVec3("material.color", Color::BLUE);
        core->glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, (void*)(2*sizeof(unsigned)));
        p->setVec3("material.color", Color::RED);
        xEnd.draw();
        p->setVec3("material.color", Color::BLUE);
        yEnd.draw();
    }
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
    indexVec = vector<unsigned int>{0, 1, 2, 3};

    float pi = asin(1)*2;
    {
    PosVec xEndPoss ={
        {m_xmax, 0, 0},
        {m_xmax - m_r, 0.5*m_r, 0},
        {m_xmax - m_r, 0.5*m_r*sin(pi/-6), 0.5*m_r*cos(pi/-6)},
        {m_xmax - m_r, 0.5*m_r*sin(pi*5/-6), 0.5*m_r*cos(pi*5/-6)}
    };
    vector<unsigned int> xEndIndex = {
        0, 1, 2,
        0, 3, 1,
        0, 2, 3,
        1, 3, 2,
    };
    VertexVec xEndVertexs;
    for(int i = 0; i < 4; i++){
        Dir dir1 = xEndPoss.at(xEndIndex.at(3*i+1)) - xEndPoss.at(xEndIndex.at(3*i));
        Dir dir2 = xEndPoss.at(xEndIndex.at(3*i+2)) - xEndPoss.at(xEndIndex.at(3*i));
        Dir norm = glm::normalize(glm::cross(dir1, dir2));
        Vertex v;
        v.vertex = xEndPoss.at(xEndIndex.at(3*i));
        v.normal = norm;
        v.coordinate = glm::vec2{0, 0};
        xEndVertexs.push_back(v);

        v.vertex = xEndPoss.at(xEndIndex.at(3*i+1));
        v.normal = norm;
        v.coordinate = glm::vec2{0, 0};
        xEndVertexs.push_back(v);

        v.vertex = xEndPoss.at(xEndIndex.at(3*i+2));
        v.normal = norm;
        v.coordinate = glm::vec2{0, 0};
        xEndVertexs.push_back(v);
    }
    xEnd = Mesh(xEndVertexs, vector<unsigned int>{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11});
    }

    {
    PosVec yEndPoss ={
        {0, m_ymax, 0},
        {0.5*m_r, m_ymax - m_r, 0},
        {0.5*m_r*cos(pi*2/-3), m_ymax - m_r, 0.5*m_r*sin(pi*2/-3)},
        {0.5*m_r*cos(pi*2/3), m_ymax - m_r, 0.5*m_r*sin(pi*2/3)}
    };
    vector<unsigned int> yEndIndex = {
        0, 1, 2,
        0, 3, 1,
        0, 2, 3,
        1, 3, 2,
    };
    VertexVec yEndVertexs;
    for(int i = 0; i < 4; i++){
        Dir dir1 = yEndPoss.at(yEndIndex.at(3*i+1)) - yEndPoss.at(yEndIndex.at(3*i));
        Dir dir2 = yEndPoss.at(yEndIndex.at(3*i+2)) - yEndPoss.at(yEndIndex.at(3*i));
        Dir norm = glm::normalize(glm::cross(dir1, dir2));
        Vertex v;
        v.vertex = yEndPoss.at(yEndIndex.at(3*i));
        v.normal = norm;
        v.coordinate = glm::vec2{0, 0};
        yEndVertexs.push_back(v);

        v.vertex = yEndPoss.at(yEndIndex.at(3*i+1));
        v.normal = norm;
        v.coordinate = glm::vec2{0, 0};
        yEndVertexs.push_back(v);

        v.vertex = yEndPoss.at(yEndIndex.at(3*i+2));
        v.normal = norm;
        v.coordinate = glm::vec2{0, 0};
        yEndVertexs.push_back(v);
    }
    yEnd = Mesh(yEndVertexs, vector<unsigned int>{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11});
    }
}
