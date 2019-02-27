#include "boundingbox.h"
#include <iostream>

using std::vector;
BoundingBox::BoundingBox():type(null)
{
    data = peakValue{0,0,0,0,0,0};
}
BoundingBox::BoundingBox(vector<Vertex>& verVec): type(null)
{
    if(verVec.size() == 0){
        return;
    }
    type = some;
    glm::vec3 firstVertex = verVec.at(0).vertex;
    float xmin = firstVertex.x;
    float xmax = xmin;
    float ymin = firstVertex.y;
    float ymax = ymin;
    float zmin = firstVertex.z;
    float zmax = zmin;
    for (auto& v : verVec) {
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
    data = peakValue{xmin, xmax, ymin, ymax, zmin, zmax};
}
glm::vec3 BoundingBox::center()
{
    assert(type != null);
    float x = (data.xmin + data.xmax) / 2;
    float y = (data.ymin + data.ymax) / 2;
    float z = (data.zmin + data.zmax) / 2;
    return glm::vec3(x, y, z);
}

BoundingBox BoundingBox::OrBox(vector<BoundingBox> boxVec){
    BoundingBox result;
    for(auto box:boxVec){
        result.OR(box);
    }
    return result;
}
void BoundingBox::setBoundingBox(BoundingBox b){
    *this = b;
}

BoundingBox& BoundingBox::OR(BoundingBox b){
    if (b.type == some){
        if(type == some){
            data.OR(b.data);
        }
        else{
            type = some;
            data = b.data;
        }

    }
    else{
        //keep type
        return *this;
    }

}
void BoundingBox::setData(peakValue d){
    type = some;
    data = d;
}

glm::vec3 BoundingBox::max(){
    return glm::vec3(data.xmax, data.ymax, data.zmax);
}
