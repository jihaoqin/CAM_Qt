#include "Mesh.h"
#include <iostream>



Mesh::Mesh(vector<Vertex> vertexs, vector<unsigned int> indexs, vector<Texture> textures)
{
	this->vertexVec = vertexs;
	this->indexVec = indexs;
	this->textureVec = textures;
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
