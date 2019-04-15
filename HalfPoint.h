#ifndef HALFPOINT_H
#define HALFPOINT_H
#include <glm/glm.hpp>
#include <string>
#include <vector>

class HalfPoint
{
public:
    glm::vec3 pos;
    std::string meshName;
};

using SuperPos = HalfPoint;
using SuperPosVec = std::vector<HalfPoint>;
#endif // HALFPOINT_H
