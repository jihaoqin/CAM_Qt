#ifndef GLBINDER_H
#define GLBINDER_H

#include <QOpenGLContext>
#include <GLProgram.h>
#include <memory>

class GLBinder
{
public:
    GLBinder();
    ~GLBinder();
    virtual void bindGL(QOpenGLContext*) = 0;
    virtual void draw(std::shared_ptr<GLProgram>) = 0;
};

#endif // OPENGLBINDER_H
