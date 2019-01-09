#ifndef LINE_H
#define LINE_H
#include "GLBinder.h"
#include <vector>
#include "Mesh.h"

class Line: public GLBinder
{
public:
    Line();
    virtual void bindGL(QOpenGLContext*) override;
    virtual void draw(QOpenGLContext*) override;
private:
    std::vector<Vertex> vertexVec;
};

#endif // LINE_H
