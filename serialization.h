#pragma once
#include <fstream>
#include <iostream>
#include <glm/glm.hpp>
#include <boost/serialization/split_free.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
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


void testVec3Save(){
    std::string path("testVec3.txt");
    std::ofstream ofile(path);
    boost::archive::text_oarchive oa(ofile);
    glm::vec3 v(1,2,3);
    oa << v;
}
void testVec3Load(){
    std::string path("testVec3.txt");
    std::ifstream ifile(path);
    if(!ifile.good()){
        std::cout<<"read failed!\n";
    }
    boost::archive::text_iarchive ia(ifile);
    glm::vec3 vi;
    ia >> vi;
}
