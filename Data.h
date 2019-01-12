#pragma once
#include "Model.h"
#include "config.h"
#include <vector>
#include "Line.h"
#include <memory>
#include "Camera2.h"

class Data
{
public:
    friend class Controller;
public:
    Data();
    void addModel(std::shared_ptr<Model>);
    void addLine(std::shared_ptr<Line>);
private:
    vector<std::shared_ptr<Model>> modelVec;
    vector<std::shared_ptr<Line>> lineVec;
    std::shared_ptr<Camera2> camera;
    BoundingBox box;
};
