#ifndef LINELIGHT_H
#define LINELIGHT_H
#include "glm/glm.hpp"
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

class LineLight
{
public:
    LineLight();
    LineLight(glm::vec3 c, glm::vec3 dir);
    glm::vec3 color;
    glm::vec3 direction;
private:
    friend class boost::serialization::access;
    template<typename Archive>
    void serialize(Archive& ar, const unsigned int version){
        ar & color & direction;
    }
};

#endif // LINELIGHT_H
