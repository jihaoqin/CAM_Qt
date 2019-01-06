#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H
#include <vector>
#include "Mesh.h"
class BoundingBox {
public:
    BoundingBox();
    BoundingBox( std::vector<Mesh>&);
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
