#include "Mesh.h"
#include <iostream>
#include <QOpenGLFunctions_4_3_Core>
#include "GLProgram.h"
Mesh::Mesh():binded(false){

}

Mesh::Mesh(vector<Vertex> vertexs, vector<unsigned int> indexs):GLMemory(),binded(false)
{
	this->vertexVec = vertexs;
	this->indexVec = indexs;
    box = BoundingBox(vertexVec);
}

Mesh::~Mesh()
{
}


void Mesh::print() {
	for (int i = 0; i < vertexVec.size(); i++) {
		auto vertex = vertexVec.at(i);
	}
}

void Mesh::bindGL(QOpenGLContext *c){
    if(binded == false){
        core = c->versionFunctions<QOpenGLFunctions_4_3_Core>();
        core->glGenVertexArrays(1, &VAO);
        core->glGenBuffers(1,&VBO);
        core->glGenBuffers(1, &EBO);
        bufferData();
        binded = true;
    }
}

void Mesh::draw(){
    if(binded == true){
        core->glBindVertexArray(VAO);
        core->glBindBuffer(GL_ARRAY_BUFFER, VBO);
        core->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        core->glDrawElements(GL_TRIANGLES, indexVec.size(), GL_UNSIGNED_INT, 0);
    }
}

BoundingBox Mesh::boundingBox(){
    return box;
}

void Mesh::setData(vector<Vertex> v, vector<unsigned int> ind){
    vertexVec = v;
    indexVec = ind;
    if(binded == true){
        bufferData();
    }
}

void Mesh::bufferData(){
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
