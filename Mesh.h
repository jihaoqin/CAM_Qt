#pragma once
#include <vector>
#include <string>
#include <3rdparty/includes/glm/glm.hpp>
#include <QOpenGLContext>
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
class Mesh
{
public:
    Mesh(vector<Vertex> , vector<unsigned int> );
	~Mesh();
public:
	void print();
    void bindGL(QOpenGLContext*);
	void draw(QOpenGLContext*);
	std::vector<Vertex> vertexVec;
    unsigned int VAO, VBO, EBO;
    std::vector<unsigned int> indexVec;
};
