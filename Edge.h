#ifndef EDGE_H
#define EDGE_H
#include <QString>
#include <glm/glm.hpp>
#include <vector>
#include "boundingbox.h"

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

template<typename F>
class RingEdge:public Edge
{
public:
    RingEdge(F& f){
        func = f;
    }
    virtual bool isOut(float u, float v){
        return f(u,v);
    }
private:
    F func;
}
#endif // EDGE_H
