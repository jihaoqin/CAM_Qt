#include "Edge.h"
#include <QStringList>
Edge::Edge():boundary(true)
{

}
Edge::Edge(std::vector<glm::vec3> points):box(),boundary(true)
{
    data(points);
}
Edge::Edge(std::function<bool(float, float)> f):boundary(true)
{
    setFunc(f);
}

glm::vec3 Edge::center(){
    return box.center();
}

QString Edge::Id(){
    return id;
}

void Edge::Id(QString s){
    id = s;
}


void Edge::data(std::vector<glm::vec3> points){
    std::vector<Vertex> vertexVec;
    for(auto i:points){
        Vertex v{i, glm::vec3{0,0,0}, glm::vec2{0,0}};
        vertexVec.push_back(v);
    }
    box = BoundingBox(vertexVec);
}

void Edge::setFunc(std::function<bool (float, float)> f){
    func = f;
}

bool Edge::isOut(float u, float v){
    return func(u, v);
}

bool Edge::isBoundary(){
    return boundary;
}

void Edge::setExtend(std::function<CPPara (CPPara, CPPara)> f){
    e = f;
}

CPPara Edge::extend(CPPara p1, CPPara p2){
    return e(p1, p2);
}

QString Edge::meshId(){
    return id.split("_").at(0);
}
