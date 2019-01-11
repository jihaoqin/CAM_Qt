#pragma once
#include <vector>
#include "vertex.h"
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
};

