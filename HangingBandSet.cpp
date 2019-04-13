#include "HangingBandSet.h"

HangingBandSet::HangingBandSet(PosVec ps)
{
    setId("post");
    setData(ps);
}

void HangingBandSet::bindGL(QOpenGLContext* c){
    if(binded == true){
        return;
    }
    else{
        core = c->versionFunctions<QOpenGLFunctions_4_3_Core>();
        core->glGenVertexArrays(1, &VAO);
        core->glGenBuffers(1,&VBO);
        core->glGenBuffers(1, &EBO);
        bufferData();
        binded = true;
    }
}
    
void HangingBandSet::draw(std::shared_ptr<GLProgram> program){
    if(binded == true && visiable == true){
        program->setMat4("model", glm::mat4(1.0));
        program->setVec3("material.color", color.rgb);
        core->glBindVertexArray(VAO);
        core->glBindBuffer(GL_ARRAY_BUFFER, VBO);
        core->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        core->glDrawElements(GL_LINES, indexVec.size(), GL_UNSIGNED_INT, 0);
    }
}

void HangingBandSet::bufferData(){
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


void HangingBandSet::setData(PosVec posVec){
    sendPoints = posVec;
    vertexVec.clear();
    for(auto p:sendPoints){
        Vertex v;
        v.vertex = p;
        v.normal = Dir{1, 0, 0};
        v.coordinate = glm::vec2{0,0};
        vertexVec.push_back(v);
    }
    indexVec.clear();
    for(int i = 0; i < sendPoints.size(); ++i){
        indexVec.push_back(i);
    }
    updateBox();
    if(binded == true){
        bufferData();
    }
}


BoundingBox HangingBandSet::boundingBox(){
    return box;
}

void HangingBandSet::updateBox(){
    box = BoundingBox(vertexVec);
}
