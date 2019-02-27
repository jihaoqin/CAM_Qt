#pragma once
#include <vector>
#include <string>
#include <3rdparty/includes/glm/glm.hpp>
#include <QOpenGLContext>
#include "GLMemory.h"
#include "vertex.h"
#include "boundingbox.h"
using std::vector;
struct Texture {
	unsigned int id;
	std::string type;
};
class Mesh:public GLMemory
{
public:
    Mesh();
    Mesh(vector<Vertex> , vector<unsigned int> );
	~Mesh();
    void setData(vector<Vertex> , vector<unsigned int>);
	void print();
    virtual void bindGL(QOpenGLContext*) override;
    virtual void draw() override;
    BoundingBox boundingBox();
protected:
	std::vector<Vertex> vertexVec;
    BoundingBox box;
};
