#pragma once
#include <vector>
#include <string>
#include <3rdparty/includes/glm/glm.hpp>
#include <QOpenGLContext>
#include "GLMemory.h"
using std::vector;
struct Texture {
	unsigned int id;
	std::string type;
};
struct Vertex {
	glm::vec3 vertex;
	glm::vec3 normal;
	glm::vec2 coordinate;
};
class Mesh:public GLMemory
{
public:
    Mesh(vector<Vertex> , vector<unsigned int> );
	~Mesh();
public:
	void print();
    virtual void bindGL(QOpenGLContext*) override;
    virtual void draw() override;
private:
	std::vector<Vertex> vertexVec;
};
