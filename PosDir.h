#ifndef POSDIR_H
#define POSDIR_H
#include <glm/glm.hpp>
#include <vector>

class PosDir
{
public:
    glm::vec3 pos;
    glm::vec3 dir;
};

using Pos = glm::vec3;
using Dir = glm::vec3;
using PosDirVec = std::vector<PosDir>;
using PosVec = std::vector<Pos>;
using DirVec = std::vector<Dir>;
#endif // POSDIR_H
