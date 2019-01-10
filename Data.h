#ifndef DATA_H
#define DATA_H

#include "Model.h"
#include "config.h"
#include <vector>
#include "Line.h"
#include <memory>
#include "Camera2.h"

class Data
{
public:
    Data();
    void addModel(std::shared_ptr<Model>);
    void addLine(std::shared_ptr<Line>);
    void draw();
private:
    vector<std::shared_ptr<Model>> modelVec;
    vector<std::shared_ptr<Line>> lineVec;
    Camera2 camera;
    BoundingBox box;
};

#endif // DATA_H
