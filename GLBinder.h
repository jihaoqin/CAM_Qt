#ifndef OPENGLBINDER_H
#define OPENGLBINDER_H

#include <QOpenGLContext>

class GLBinder
{
public:
    GLBinder();
    ~GLBinder();
    virtual void bindGL(QOpenGLContext*) = 0;
    virtual void draw() = 0;
};

#endif // OPENGLBINDER_H
