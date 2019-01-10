#ifndef GLMEMORY_H
#define GLMEMORY_H

#include <QOpenGLContext>
#include <QOpenGLFunctions_4_3_Core>
#include "GLBinder.h"

class GLMemory: public GLBinder
{
public:
    GLMemory();
    ~GLMemory();
protected:
    unsigned int VAO,VBO,EBO;
    std::vector<unsigned int> indexVec;
    QOpenGLContext* context;
    QOpenGLFunctions_4_3_Core *core;
};

#endif // GLMEMORY_H
