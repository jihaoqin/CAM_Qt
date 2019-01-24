#pragma once
#include <glm/glm.hpp>
#include <boost/serialization/split_free.hpp>
BOOST_SERIALIZATION_SPLIT_FREE(glm::vec3);
namespace boost {
namespace serialization {

template <typename Archive>
void save(Archive& ar, const glm::vec3& v, unsigned int version){
    float x = v.x;
    float y = v.y;
    float z = v.z;
    ar & x;
    ar & y;
    ar & z;
}
}
template <typename Archive>
void load(Archive& ar, glm::vec3& v, unsigned int version){
    float x,y,z;
    ar & x & y & z;
    v = glm::vec3(x, y, z);
}
}
