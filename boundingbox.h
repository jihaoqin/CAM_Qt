#pragma once
#include <vector>
#include "vertex.h"
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
class BoundingBox {
public:
    static BoundingBox OrBox(std::vector<BoundingBox>);
    BoundingBox();
    BoundingBox( std::vector<Vertex>&);
    void setBoundingBox(BoundingBox);
    glm::vec3 center();
    double xmin;
    double xmax;
    double ymin;
    double ymax;
    double zmin;
    double zmax;
    void print();
    friend class boost::serialization::access;
    template<typename Archive>
    void serialize(Archive& ar, const unsigned int version){
        ar&xmin&xmax&ymin&ymax&zmin&zmax;
    }
};

