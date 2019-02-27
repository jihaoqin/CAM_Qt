#ifndef GLMEMORY_H
#define GLMEMORY_H

#include <QOpenGLContext>
#include <QOpenGLFunctions_4_3_Core>
#include "DataObject.h"

class GLMemory
{
public:
    GLMemory();
    ~GLMemory();
    virtual void bindGL(QOpenGLContext*) = 0;
    virtual void draw() = 0;
protected:
    unsigned int VAO,VBO,EBO;
    std::vector<unsigned int> indexVec;
    QOpenGLContext* context;
    QOpenGLFunctions_4_3_Core *core;
};

#endif // GLMEMORY_H
