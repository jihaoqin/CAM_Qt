#include "Point.h"

Point::Point(glm::vec3 p):GLMemory()
{
    pos = p;
    Vertex v;
    v.normal = glm::vec3(1,0,0);
    v.vertex = p;
    v.coordinate = glm::vec2(0,0);
    vertexVec.push_back(v);
    indexVec.push_back(0);
}

void Point::bindGL(QOpenGLContext *c){
    core = c->versionFunctions<QOpenGLFunctions_4_3_Core>();
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

void Point::draw(std::shared_ptr<GLProgram> program){
    if(binded == false){
        assert(binded == true);
        return;
    }
    core->glBindVertexArray(VAO);
    core->glBindBuffer(GL_ARRAY_BUFFER, VBO);
    core->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //core->glDrawElements(GL_TRIANGLES, indexVec.size(), GL_UNSIGNED_INT, 0);
    //core->glDrawElements(GL_POINTS, indexVec.size(), GL_UNSIGNED_INT, 0);
    core->glPointSize(6);
    core->glDrawArrays(GL_POINTS, 0 ,1);
}
