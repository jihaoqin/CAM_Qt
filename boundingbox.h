#pragma once
#include <vector>
#include "vertex.h"
struct peakValue{
    peakValue& OR(peakValue p){
        auto MIN = [](float a, float b)->float {return a < b ? a : b; };
        auto MAX = [](float a, float b)->float {return a > b ? a : b; };
        xmin = MIN(xmin, p.xmin);
        xmax = MAX(xmax, p.xmax);
        ymin = MIN(ymin, p.ymin);
        ymax = MAX(ymax, p.ymax);
        zmin = MIN(zmin, p.zmin);
        zmax = MAX(zmax, p.zmax);
        return *this;
    }
    float xmin;
    float xmax;
    float ymin;
    float ymax;
    float zmin;
    float zmax;
};
class BoundingBox {
public:
    enum Type{
        null,
        some
    };
    static BoundingBox OrBox(std::vector<BoundingBox>);
    BoundingBox();
    BoundingBox( std::vector<Vertex>&);
    peakValue peak();
    peakValue peak(peakValue);
    BoundingBox& OR(BoundingBox);
    void setData(peakValue);
    glm::vec3 max();
    void setBoundingBox(BoundingBox);
    glm::vec3 center();
    Type type;
private:
    peakValue data;
};

