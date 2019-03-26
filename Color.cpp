#include "Color.h"

glm::vec3 Color::RED(1.0, 0, 0);
glm::vec3 Color::GREEN(0, 1.0, 0);
glm::vec3 Color::BLACK(0, 0, 0);
glm::vec3 Color::YELLOW(1.0, 1.0, 0);
glm::vec3 Color::BLUE(0,0,1);

Color::Color()
{

}

Color::Color(glm::vec3 c){
    rgb = c;
}
