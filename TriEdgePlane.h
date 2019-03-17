#ifndef TRIEDGEPLANE_H
#define TRIEDGEPLANE_H
#include "DataObject.h"
#include <glm/glm.hpp>
#include "HalfPoint.h"
#include "Mesh.h"
class Edge;
using EdgePtr = std::shared_ptr<Edge>;

class TriEdgePlane:public DataObject
{
public:
    friend class TriEdgePlaneAssist;
    friend class TriCurveAssist;
    TriEdgePlane(QString s, float radius, glm::vec3 loc, glm::vec3 uDirection, glm::vec3 normDir);
    TriEdgePlane();
    void initial(float radius, glm::vec3 loc, glm::vec3 uDirection, glm::vec3 normDir);
    virtual void bindGL(QOpenGLContext *) override;
    virtual void draw(std::shared_ptr<GLProgram>) override;
    BoundingBox boundingBox();
    std::vector<HalfPoint> intersectionPoints(glm::vec3 begin, glm::vec3 dir);
private:
    void initialMesh();
    float R;
    glm::vec3 uDir;
    glm::vec3 anchor;
    glm::vec3 norm;
    std::vector<EdgePtr> edges;
    Mesh m;
};

#endif // TRIEDGEPLANE_H
