#ifndef POINT_H
#define POINT_H
#include <glm/glm.hpp>
#include "DataObject.h"
#include "vertex.h"
#include <vector>
#include "Color.h"
#include "rapidjson/prettywriter.h"

using namespace rapidjson;

class Point:public DataObject
{
public:
    Point(glm::vec3 p, const char* c);
    virtual ~Point();
    void bindGL(QOpenGLContext*) override;
    virtual void draw(std::shared_ptr<GLProgram>) override;
    virtual glm::vec3 getPos();
    void setPos(glm::vec3);
    bool picked;
    const char* meshId();
    void meshId(const char*);

    virtual void serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const override;
protected:
    void updateSelf() override;
    glm::vec3 pos;
    std::string meshName;
private:
    void updateBoundingBox();
    std::vector<Vertex> vertexVec;
    std::vector<unsigned int> indexVec;
    QOpenGLFunctions_4_3_Core *core;
    unsigned int VAO,VBO,EBO;
    Color color;
};

using PointPtr = std::shared_ptr<Point>;
using PointWptr = std::weak_ptr<Point>;
#endif // POINT_H
