#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H
#include <vector>
#include "Mesh.h"
class BoundingBox {

public:
    static BoundingBox OrBox(vector<BoundingBox>);
    BoundingBox();
    BoundingBox( std::vector<Mesh>&);
    BoundingBox(double, double, double, double, double, double);
    glm::vec3 center();
    double xmin;
    double xmax;
    double ymin;
    double ymax;
    double zmin;
    double zmax;
    void print();
};

#endif // BOUNDINGBOX_H
