#pragma once
#include "serialization.h"
#include <fstream>
#include <string>
#include <iostream>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

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

void testMat4Serialization(){
    std::string path("testMat4.txt");
    std::ofstream ofile(path);
    boost::archive::text_oarchive oa(ofile);
    glm::mat4 m(1.0);
    oa<<m;
    ofile.close();

    std::ifstream ifile(path);
    boost::archive::text_iarchive ia(ifile);
    glm::mat4 v(2.9);
    ia>>v;
}


void MainTest(){
    testMat4Serialization();
}
