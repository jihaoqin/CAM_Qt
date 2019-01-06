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
	std::vector<Vertex> vertexVec;
    unsigned int VAO, VBO, EBO;
private:
    std::vector<unsigned int> indexVec;
    bool binded;
};

vector<Vertex> arrayToVec(unsigned int size, const float a[]);
vector<unsigned int> initVecUint(unsigned int size);
