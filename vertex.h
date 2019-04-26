#ifndef VERTEX_H
#define VERTEX_H
#include <glm/glm.hpp>
#include <vector>

struct Vertex {
    glm::vec3 vertex;
    glm::vec3 normal;
    glm::vec2 coordinate;
};
using VertexVec = std::vector<Vertex>;
#endif // VERTEX_H
