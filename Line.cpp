#include "Line.h"

Line::Line()
{
    float R = 100.1;
    int sideNum = 100;
    for(int i =0; i < sideNum; i++){
        float t = 2*3.14/100*i;
        Vertex v1, v2;

        v1.vertex = glm::vec3(R*cos(t), R*sin(t), 200);
        v1.normal = glm::vec3(R*cos(t), R*sin(t), 0);
        v1.coordinate = glm::vec2(0.0);

        v2.vertex = glm::vec3(R*cos(t), R*sin(t), 210);
        v2.normal = glm::vec3(R*cos(t), R*sin(t), 0);
        v2.coordinate = glm::vec2(0.0);

        vertexVec.push_back(v1);
        vertexVec.push_back(v2);
    }
    for(int i = 0; i < sideNum - 1; i++){
        indexVec.push_back(2*i);
        indexVec.push_back(2*i+1);
        indexVec.push_back(2*i+2);
        indexVec.push_back(2*i+1);
        indexVec.push_back(2*i+3);
        indexVec.push_back(2*i+2);
    }
}

void Line::bindGL(QOpenGLContext* context){
    core = context->versionFunctions<QOpenGLFunctions_4_3_Core>();

    core->glGenVertexArrays(1, &VAO);
    core->glBindVertexArray(VAO);
    core->glGenBuffers(1,&VBO);
    core->glBindBuffer(GL_ARRAY_BUFFER, VBO);
    core->glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vertexVec.size(), &(vertexVec[0]), GL_STATIC_DRAW);
    core->glGenBuffers(1, &EBO);
    core->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    core->glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*indexVec.size(), &(indexVec[0]), GL_STATIC_DRAW);

    core->glEnableVertexAttribArray(0);
    core->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
    core->glEnableVertexAttribArray(1);
    core->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(3*sizeof(float)));
    core->glEnableVertexAttribArray(2);
    core->glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(6*sizeof(float)));
    binded = true;
}
void Line::draw(std::shared_ptr<GLProgram> program){
    core->glBindVertexArray(VAO);
    core->glBindBuffer(GL_ARRAY_BUFFER, VBO);
    core->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    core->glDrawElements(GL_TRIANGLES, indexVec.size(), GL_UNSIGNED_INT, 0);
}
