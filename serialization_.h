#pragma once

#include <glm/glm.hpp>
#include <boost/serialization/split_free.hpp>

BOOST_SERIALIZATION_SPLIT_FREE(glm::mat4);
namespace boost {
namespace serialization {
template <typename Archive>
void save(Archive& ar, const glm::mat4& m, unsigned int version){
    ar & m[0][0]& m[1][0]&m[2][0]&m[3][0];
    ar & m[0][1]& m[1][1]&m[2][1]&m[3][1];
    ar & m[0][2]& m[1][2]&m[2][2]&m[3][2];
    ar & m[0][3]& m[1][3]&m[2][3]&m[3][3];
}

template <typename Archive>
void load(Archive& ar, glm::mat4& m, unsigned int version){
    ar & m[0][0]& m[1][0]&m[2][0]&m[3][0];
    ar & m[0][1]& m[1][1]&m[2][1]&m[3][1];
    ar & m[0][2]& m[1][2]&m[2][2]&m[3][2];
    ar & m[0][3]& m[1][3]&m[2][3]&m[3][3];
}

}

}