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

using PosDirVec = std::vector<PosDir>;

#endif // POSDIR_H
