#ifndef LINELIGHT_H
#define LINELIGHT_H
#include "glm/glm.hpp"

class LineLight
{
public:
    LineLight();
    LineLight(glm::vec3 c, glm::vec3 dir);
    glm::vec3 color;
    glm::vec3 direction;
};

#endif // LINELIGHT_H
