#ifndef RING_H
#define RING_H
#include "Mesh.h"
#include "glm/glm.hpp"
#include <vector>
#include "DataObject.h"
#include "utility.h"
#include "HalfPoint.h"

class RingAssist;
class RingCurveAssist;
class Edge;
using EdgePtr = std::shared_ptr<Edge>;
class Ring:public DataObject
{
public:
    friend class RingAssist;
    friend class RingCurveAssist;
    Ring(QString s, double R_, double r_, double angle_ , glm::vec3 anchor_, glm::vec3 zdir_, glm::vec3 xdir_);
    virtual ~Ring();
    virtual void bindGL(QOpenGLContext *) override;
    virtual void draw(std::shared_ptr<GLProgram>) override;
    BoundingBox boundingBox();
    vector<HalfPoint> intersectionPoints(glm::vec3 begin, glm::vec3 dir);
    vector<EdgePtr> getEdges();
private:
    vector<Vertex> generateEdge();
    vector<EdgePtr> edges;
    double R;
    double r;
    double angle;//弧度制
    glm::vec3 anchor;
    glm::vec3 zdir;
    glm::vec3 xdir;
    Mesh m;

};

#endif // RING_H
