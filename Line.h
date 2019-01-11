#ifndef LINE_H
#define LINE_H
#include <vector>
#include "Mesh.h"
#include "GLMemory.h"


class Line: public GLMemory
{
public:
    Line();
    virtual void bindGL(QOpenGLContext*) override;
    virtual void draw(std::shared_ptr<GLProgram>) override;
private:
    std::vector<Vertex> vertexVec;

};

#endif // LINE_H
