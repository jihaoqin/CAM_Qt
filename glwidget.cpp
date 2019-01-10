#include "glwidget.h"
#include "config.h"
#include <QDebug>
#include <QMouseEvent>
#include <iostream>
#include <QOpenGLContext>
#include <QOpenGLFunctions_4_3_Core>


GLWidget::GLWidget(QWidget *parent):QOpenGLWidget (0), model(MODEL_PATH), camera(model.boundingBox()),context(0),line()
{
    QSurfaceFormat fmt;
    fmt.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(fmt);
    setFormat(fmt);
}

void GLWidget::initializeGL(){

    makeCurrent();
    initializeOpenGLFunctions();
    context = QOpenGLContext::currentContext();
    glClearColor(0, 0, 0, 1);
    if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, VERTEX_PATH)){
        qDebug()<<"Failed to load vertexShader: "<<program.log()<<"\n";
    }else{
        // do nothing
    }
    if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment, FRAGMENT_PATH)){
        qDebug()<<"Failed to load fragmentShader: "<<program.log()<<"\n";
    }else{
        // do nothing
    }
    program.link();
    program.bind();
    glEnable(GL_DEPTH_TEST);
    ctrl->addModel(MODEL_PATH);
    ctrl->addLine();
    //model.bindGL(context);
    //line.bindGL(context);
}

void GLWidget::paintGL(){
    program.bind();
    glClearColor(0.2f,0.3f,0.3f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    setMat4("model", model.modelMatrix());
    setMat4("view", camera.viewMatrix());
    setMat4("perspective", camera.perspectiveMatrix());
    setVec4("color", glm::vec4(1.0f, 0.0f,0.5f, 1.0f));
    ctrl->draw();
    update();
}

void GLWidget::resizeGL(int w, int h){
    camera.viewPortRatio(w, h);
    glViewport(0,0,w,h);
}


void GLWidget::setMat4(const char* name, const glm::mat4 t){
    GLuint location = program.uniformLocation(name);
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(t));
}

void GLWidget::mousePressEvent(QMouseEvent *event){
    mLastPos = event->pos();
}
void GLWidget::mouseMoveEvent(QMouseEvent *event){
    if (event->buttons() & Qt::MidButton) {
        QPoint mPos = event->pos();
        float deltaX = mPos.x() - mLastPos.x();
        float deltaY = mPos.y() - mLastPos.y();
        camera.processMouseMove(deltaX, deltaY);
        update();
        mLastPos = mPos;
    }
}

void GLWidget::wheelEvent(QWheelEvent *event){
   auto degree = event->angleDelta();
   camera.processScroll(degree.y());
   update();
}
void GLWidget::setVec4(const char *name, const glm::vec4 value){
    GLuint location = program.uniformLocation(name);
    glUniform4fv(location, 1, glm::value_ptr(value));
}
