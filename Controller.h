#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "Data.h"
#include <string>
#include <QOpenGLContext>
#include "GLProgram.h"
#include <QOpenGLFunctions_4_3_Core>
#include <memory>

class Controller
{
public:
    Controller();
    void addModel(std::string);
    void addLine();
    void bindGL(QOpenGLContext*, std::shared_ptr<GLBinder>);
    void initialGLVar(QOpenGLContext*, std::shared_ptr<GLProgram>);
    void draw();
private:
    Data *data;
    QOpenGLContext* context;
    std::shared_ptr<GLProgram> program;
    QOpenGLFunctions_4_3_Core* core;
};

#endif // CONTROLLER_H
