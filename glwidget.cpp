#include "glwidget.h"

#include <QDebug>
#include <iostream>
#include <QOpenGLContext>
#include <QOpenGLFunctions_4_3_Core>
const char *vertexShaderSource =
        "#version 330 core\n"
        "layout(location = 0) in vec3 aPos;\n"
        "void main(){\n"
        "  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\n\0";
const char *fragmentShaderSource =
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main(){\n"
        "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\n\0";


GLWidget::GLWidget(QWidget *parent):QOpenGLWidget (0), point{0.5f,0.5f,0.5f, 0.2f,0.1f,0.4f, -0.3f,0.0f,0.1f},model("E:\\0qin\\Qt\\CAM_Qt\\3.STL")
{
    QSurfaceFormat fmt;
    fmt.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(fmt);
    setFormat(fmt);
}

void GLWidget::initializeGL(){

    makeCurrent();
    initializeOpenGLFunctions();
    glClearColor(0, 0, 0, 1);
    QOpenGLShaderProgram program;
    if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, "E:\\0qin\\Qt\\CAM_Qt\\vertexShader.vs")){
        qDebug()<<"Failed to load vertexShader: "<<program.log()<<"\n";
    }else{
        // do nothing
    }
    if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment, "E:\\0qin\\Qt\\CAM_Qt\\loadingModel.fs")){
        qDebug()<<"Failed to load fragmentShader: "<<program.log()<<"\n";
    }else{
        // do nothing
    }
    program.link();
    program.bind();
    bindGL(model);
}

void GLWidget::paintGL(){
    program.bind();
    QOpenGLContext *c = QOpenGLContext::currentContext();
    glClearColor(0.2f,0.3f,0.3f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    model.draw(c);
}

void GLWidget::resizeGL(int w, int h){
    glViewport(0,0,w,h);
}

void GLWidget::bindGL(Mesh& mesh){
    makeCurrent();
    QOpenGLContext *c = QOpenGLContext::currentContext();
    mesh.bindGL(c);
}
void GLWidget::bindGL(Model& model){
    makeCurrent();
    QOpenGLContext *c = QOpenGLContext::currentContext();
    model.bindGL(c);
}

