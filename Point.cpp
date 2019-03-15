#include "Point.h"

Point::Point(glm::vec3 p, const char* c):color(Color::GREEN), picked(false)
{
    setId(c);
    pos = p;
    Vertex v;
    v.normal = glm::vec3(1,0,0);
    v.vertex = p;
    v.coordinate = glm::vec2(0,0);
    vertexVec.push_back(v);
    indexVec.push_back(0);
    updateBoundingBox();
}

void Point::bindGL(QOpenGLContext *c){
    core = c->versionFunctions<QOpenGLFunctions_4_3_Core>();
    core->glGenVertexArrays(1, &VAO);
    core->glBindVertexArray(VAO);
    core->glGenBuffers(1,&VBO);
    core->glGenBuffers(1, &EBO);
    binded = true;
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

void Point::draw(std::shared_ptr<GLProgram> program){
    if(visiable == false){
        return;
    }
    program->setMat4("model", glm::mat4(1.0));
    program->setVec3("material.color", color.rgb);
    core->glBindVertexArray(VAO);
    core->glBindBuffer(GL_ARRAY_BUFFER, VBO);
    core->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //core->glDrawElements(GL_TRIANGLES, indexVec.size(), GL_UNSIGNED_INT, 0);
    //core->glDrawElements(GL_POINTS, indexVec.size(), GL_UNSIGNED_INT, 0);
    core->glPointSize(4);
    core->glDrawArrays(GL_POINTS, 0 ,1);
}

void Point::updateBoundingBox(){
    box.setData(peakValue{pos.x, pos.x, pos.y, pos.y, pos.z, pos.z});
}

Point::~Point(){

}

glm::vec3 Point::getPos(){
    return pos;
}

void Point::setPos(glm::vec3 p){
    pos = p;
    update();
}

void Point::updateSelf(){
    Vertex v;
    v.normal = glm::vec3(1,0,0);
    v.vertex = pos;
    v.coordinate = glm::vec2(0,0);
    vertexVec.at(0) = v;
    if(binded == false){
        return ;
    }
    else{
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
}

const char* Point::meshId(){
    return meshName.c_str();
}
void Point::meshId(const char* s){
    meshName = std::string(s);
}
