#pragma once
#include <vector>
#include <string>
#include "Shader.h"
#include <3rdparty/includes/glm/glm.hpp>
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
    Mesh(vector<Vertex> , vector<unsigned int> , vector<Texture> );
	~Mesh();
	void print();
	std::vector<Vertex> vertexVec;
private:
	unsigned int VAO, VBO, EBO;
	std::vector<unsigned int> indexVec;
	std::vector<Texture> textureVec;
};

vector<Vertex> arrayToVec(unsigned int size, const float a[]);
vector<unsigned int> initVecUint(unsigned int size);
