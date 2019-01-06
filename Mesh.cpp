#include "Mesh.h"
#include <iostream>
#include <QOpenGLFunctions_4_3_Core>


Mesh::Mesh(vector<Vertex> vertexs, vector<unsigned int> indexs):binded(false)
{
	this->vertexVec = vertexs;
	this->indexVec = indexs;
}

Mesh::~Mesh()
{
}


void Mesh::print() {
	for (int i = 0; i < vertexVec.size(); i++) {
		auto vertex = vertexVec.at(i);
		//std::cout << "point = " << vertex.vertex.x << ", " << vertex.vertex.y << ", " << vertex.vertex.z << "\n";
	}
}

void Mesh::bindGL(QOpenGLContext *c){
    QOpenGLFunctions_4_3_Core *core = c->versionFunctions<QOpenGLFunctions_4_3_Core>();
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
}
vector<Vertex> arrayToVec(unsigned int size, const float a[]) {
	int num = size / sizeof(float);
	Vertex x;
	vector<Vertex> v;
	for (int i = 0; i < num; i = i+8) {
		x.vertex = glm::vec3(a[i], a[i+1], a[i+2]);
		x.normal = glm::vec3(a[i+3], a[i+4], a[i+5]);
		x.coordinate = glm::vec2(a[i+6], a[i+7]);
		v.push_back(x);
	}
	return v;
}

vector<unsigned int> initVecUint(unsigned int num) {
	num = num / 8;
	vector<unsigned int> uintVec;
	for (int i = 0; i < num; i++) {
		uintVec.push_back(i);
	}
	return uintVec;
}
