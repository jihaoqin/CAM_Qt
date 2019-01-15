#include "LineLight.h"

LineLight::LineLight(){

}

LineLight::LineLight(glm::vec3 c, glm::vec3 dir){
    color = c;
    direction = dir;
}
