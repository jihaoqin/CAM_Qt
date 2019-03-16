#ifndef EDGE_H
#define EDGE_H
#include <QString>
#include <glm/glm.hpp>
#include <vector>
#include "boundingbox.h"
#include <functional>

class Edge
{
public:
    Edge();
    Edge(std::vector<glm::vec3>);
    QString Id();
    void Id(QString);
    glm::vec3 center();
    void data(std::vector<glm::vec3>);
    virtual bool isOut(float u, float v) = 0;
private:
    BoundingBox box;
    QString id;
};


class RingEdge:public Edge
{
public:
    RingEdge(std::function<bool (float, float)> f);
    virtual bool isOut(float u, float v);
private:
    std::function<bool (float, float)> func;
};
#endif // EDGE_H
