#ifndef OPENGLBINDER_H
#define OPENGLBINDER_H

#include <QOpenGLContext>
#include <QOpenGLFunctions_4_3_Core>

class GLBinder
{
public:
    GLBinder();
    virtual ~GLBinder();
    virtual void bindGL(QOpenGLContext*) = 0;
    virtual void draw(QOpenGLContext*) = 0;
protected:
    unsigned int VAO,VBO,EBO;
    std::vector<unsigned int> indexVec;
    QOpenGLContext* context;
    QOpenGLFunctions_4_3_Core *core;
};

#endif // OPENGLBINDER_H
