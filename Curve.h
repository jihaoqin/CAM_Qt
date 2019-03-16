#ifndef CURVE_H
#define CURVE_H
#include "DataObject.h"
#include "PosDir.h"
#include <vector>
#include "Color.h"
#include <glm/glm.hpp>

class Curve : public DataObject
{
public:
    enum class Type{
        ringCurve,
        tCurve,
        general
    };
    Curve(std::vector<glm::vec3>, const char*);
    Curve(const char*);
    void data(std::vector<glm::vec3>);
    virtual void bindGL(QOpenGLContext*) override;
    virtual void draw(std::shared_ptr<GLProgram>) override;
    Type type;
private:
    void bufferData();
    void initial(std::vector<glm::vec3>);
    std::vector<Vertex> vertexVec;
    std::vector<unsigned int> indexVec;
    QOpenGLFunctions_4_3_Core *core;
    unsigned int VAO,VBO,EBO;
    Color color;
};

#endif // CURVE_H