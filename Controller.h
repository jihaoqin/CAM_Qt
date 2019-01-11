#pragma once
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
    void updateBoundingBox();
    void bindData(std::shared_ptr<Data>);
    void addModel(std::string);
    void addLine();
    void bindGL(QOpenGLContext*, std::shared_ptr<GLBinder>);
    void initialGLVar(QOpenGLContext*, std::shared_ptr<GLProgram>);
    void draw(std::shared_ptr<GLProgram>);
    std::shared_ptr<Camera2> getCamera();
private:
    std::shared_ptr<Data> data;
    QOpenGLContext* context;
    std::shared_ptr<GLProgram> program;
    QOpenGLFunctions_4_3_Core* core;
};

