#ifndef EDGE_H
#define EDGE_H
#include <QString>
#include <glm/glm.hpp>
#include <vector>
#include "boundingbox.h"
#include <functional>
#include <memory>

class Edge;
using EdgePtr = std::shared_ptr<Edge>;

class Edge
{
public:
    Edge();
    Edge(std::function<bool(float, float)> f);
    Edge(std::vector<glm::vec3>);
    QString Id();
    void Id(QString);
    glm::vec3 center();
    void data(std::vector<glm::vec3>);
    void setFunc(std::function<bool (float, float)> f);
    virtual bool isOut(float u, float v);
    bool isBoundary();
private:
    bool boundary;
    BoundingBox box;
    QString id;
    std::function<bool (float, float)> func;
};



/*
class RingEdge:public Edge
{
public:
    RingEdge(std::function<bool (float, float)> f);
    virtual bool isOut(float u, float v);
private:
    std::function<bool (float, float)> func;
};

*/
#endif // EDGE_H
