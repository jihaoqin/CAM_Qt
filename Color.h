#ifndef COLOR_H
#define COLOR_H
#include <glm/glm.hpp>

class Color
{
public:
    static glm::vec3 RED;
    static glm::vec3 YELLOW;
    static glm::vec3 GREEN;
    static glm::vec3 BLACK;
    Color();
    Color(glm::vec3);
    glm::vec3 rgb;
};

#endif // COLOR_H
