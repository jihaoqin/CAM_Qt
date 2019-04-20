#ifndef EDGE_H
#define EDGE_H
#include <QString>
#include <glm/glm.hpp>
#include <vector>
#include "boundingbox.h"
#include <functional>
#include <memory>
#include "CPPara.h"

class Edge;
using EdgePtr = std::shared_ptr<Edge>;
using EdgePtrVec = std::vector<EdgePtr>;
class Edge
{
public:
    Edge();
    Edge(std::function<bool(float, float)> f);
    Edge(std::vector<glm::vec3>);
    QString Id();
    void Id(QString);
    QString meshId();
    glm::vec3 center();
    void data(std::vector<glm::vec3>);
    void setFunc(std::function<bool (float, float)> f);
    void setExtend(std::function<CPPara (CPPara, CPPara)> f);
    virtual bool isOut(float u, float v);
    virtual CPPara extend(CPPara, CPPara);
    bool isBoundary();
private:
    bool boundary;
    BoundingBox box;
    QString id;
    std::function<bool (float, float)> func;
    std::function<CPPara (CPPara, CPPara)> e;
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
