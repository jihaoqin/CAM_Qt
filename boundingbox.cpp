#include "boundingbox.h"
#include <iostream>

BoundingBox::BoundingBox() {
    xmin = 0;
    xmax = 0;
    ymin = 0;
    ymax = 0;
    zmin = 0;
    zmax = 0;
}
BoundingBox::BoundingBox(vector<Mesh>& meshVec) {
    assert(meshVec.size()>0);
    glm::vec3 firstVertex = meshVec.at(0).vertexVec.at(0).vertex;
    xmin = firstVertex.x;
    xmax = xmin;
    ymin = firstVertex.y;
    ymax = ymin;
    zmin = firstVertex.z;
    zmax = zmin;
    for (auto& m : meshVec) {
        for (auto& v : m.vertexVec) {
            float x = v.vertex.x;
            float y = v.vertex.y;
            float z = v.vertex.z;
            auto MIN = [](float a, float b)->float {return a < b ? a : b; };
            auto MAX = [](float a, float b)->float {return a > b ? a : b; };
            xmin = MIN(xmin, x);
            xmax = MAX(xmax, x);
            ymin = MIN(ymin, y);
            ymax = MAX(ymax, y);
            zmin = MIN(zmin, z);
            zmax = MAX(zmax, z);
        }
    }
#if DEBUG
    print();
#endif
}
glm::vec3 BoundingBox::center()
{
    float x = (xmin + xmax) / 2;
    float y = (ymin + ymax) / 2;
    float z = (zmin + zmax) / 2;
    return glm::vec3(x, y, z);
}
void BoundingBox::print() {
    std::cout << "[xmin, xmax, ymin, ymax] = ["<<xmin << ", "<<xmax<<", "<<ymin<<", "<<ymax<<"]";
}

BoundingBox BoundingBox::OrBox(vector<BoundingBox> boxVec){
    assert(boxVec.size()>0);
    BoundingBox f = boxVec.at(0);
    double xmin = f.xmin;
    double xmax = f.xmax;
    double ymin = f.ymin;
    double ymax = f.ymax;
    double zmin = f.zmin;
    double zmax = f.zmax;
    for(auto b:boxVec){
            auto MIN = [](float a, float b)->float {return a < b ? a : b; };
            auto MAX = [](float a, float b)->float {return a > b ? a : b; };
            xmin = MIN(xmin, b.xmin);
            xmax = MAX(xmax, b.xmax);
            ymin = MIN(ymin, b.ymin);
            ymax = MAX(ymax, b.ymax);
            zmin = MIN(zmin, b.zmin);
            zmax = MAX(zmax, b.zmax);
    }
    BoundingBox result;
    result.xmin = xmin;
    result.xmax = xmax;
    result.ymin = ymin;
    result.ymax = ymax;
    result.zmin = zmin;
    result.zmax = zmax;
    return result;
}
