#include "glwidget.h"
#include "config.h"
#include <QDebug>
#include <QMouseEvent>
#include <QKeyEvent>
#include <iostream>
#include <QOpenGLContext>
#include <QOpenGLFunctions_4_3_Core>
#include <QApplication>


GLWidget::GLWidget(QWidget *parent):QOpenGLWidget (0), context(0)
{
    //初始化camera
    QSurfaceFormat fmt;
    fmt.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(fmt);
    setFormat(fmt);
}

void GLWidget::initializeGL(){

    makeCurrent();
    initializeOpenGLFunctions();
    glClearColor(0.2f,0.3f,0.3f,1.0f);
    context = QOpenGLContext::currentContext();
    program = std::make_shared<GLProgram>(context);
    ctrl->initialGLVar(context, program);
    camera = ctrl->getCamera();
    if (!program->addShaderFromSourceFile(QOpenGLShader::Vertex, VERTEX_PATH)){
        qDebug()<<"Failed to load vertexShader: "<<program->log()<<"\n";
    }else{
        // do nothing
    }
    if (!program->addShaderFromSourceFile(QOpenGLShader::Fragment, FRAGMENT_PATH)){
        qDebug()<<"Failed to load fragmentShader: "<<program->log()<<"\n";
    }else{
        // do nothing
    }
    program->link();
    program->bind();
    glEnable(GL_DEPTH_TEST);
    ctrl->addModel(MODEL_PATH);
    ctrl->addLine();
    ctrl->addTee();
    camera->bindBoundingBox(ctrl->updateBoundingBox());
}

void GLWidget::paintGL(){
    program->bind();
    glClearColor(0.2f,0.3f,0.3f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glViewport(0, 0, width(), height());
    ctrl->draw(program);
    update();
}

void GLWidget::resizeGL(int w, int h){
    makeCurrent();
    camera->viewPortRatio(w, h);
}

void GLWidget::mousePressEvent(QMouseEvent *event){
    mLastPos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event){
    if (event->buttons() & Qt::MidButton) {
        //按下了鼠标中键
        if(QApplication::keyboardModifiers() == Qt::ControlModifier){
            //按下了control键
            QPoint mPos = event->pos();
            glm::vec4 viewPort(0,0,width(),height());
            camera->processTranslation(mPos, mLastPos, viewPort);
            update();
            mLastPos = mPos;
        }
        else{
            QPoint mPos = event->pos();
            float deltaX = mPos.x() - mLastPos.x();
            float deltaY = mPos.y() - mLastPos.y();
            //camera->processRotation(deltaX, deltaY);
            glm::vec4 viewPort(0,0,width(),height());
            camera->processRotation(mPos, mLastPos, viewPort);
            update();
            mLastPos = mPos;
        }
    }
}

void GLWidget::wheelEvent(QWheelEvent *event){
   auto degree = event->angleDelta();
   camera->processScroll(degree.y());
   update();
}

void GLWidget::bindController(Controller *c){
    ctrl = c;
}
