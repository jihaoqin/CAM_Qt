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
    Curve(std::vector<glm::vec3>, const char*);
    Curve(const char*);
    void data(std::vector<glm::vec3>);
    virtual void bindGL(QOpenGLContext*) override;
    virtual void draw(std::shared_ptr<GLProgram>) override;
    virtual void setWindingPara(float angle, float coe);
    virtual void setWindingAngle(float angle);
    void setShowRange(unsigned int, unsigned int);
    void resetShowRange();
    void setColor(Color);
private:
    void bufferData();
    void initial(std::vector<glm::vec3>);
    unsigned int m_beginInd;
    unsigned int m_size;
    std::vector<Vertex> vertexVec;
    std::vector<unsigned int> indexVec;
    QOpenGLFunctions_4_3_Core *core;
    unsigned int VAO,VBO,EBO;
    Color color;
};

using CurvePtr = std::shared_ptr<Curve>;
#endif // CURVE_H
