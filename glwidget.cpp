#include "glwidget.h"
#include <QDebug>
#include <QMouseEvent>
#include <QKeyEvent>
#include <iostream>
#include <QOpenGLContext>
#include <QOpenGLFunctions_4_3_Core>
#include <QApplication>
#include "GuiConnector.h"
#include <QMouseEvent>

GLWidget::GLWidget(QWidget *parent):QOpenGLWidget (parent), context(0)
{
    setMinimumSize(400,300);
    QSurfaceFormat fmt;
    fmt.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(fmt);
    setFormat(fmt);
}

GLWidget::~GLWidget(){

}

void GLWidget::initializeGL(){

    makeCurrent();
    initializeOpenGLFunctions();
    glClearColor(0.2f,0.3f,0.3f,1.0f);
    context = QOpenGLContext::currentContext();
    meshProgram = std::make_shared<GLProgram>(context, GLProgram::Mesh);
    pointProgram = std::make_shared<GLProgram>(context, GLProgram::Point);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
}

void GLWidget::paintGL(){
    glClearColor(0.2f,0.3f,0.3f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glViewport(0, 0, width(), height());
    ctrl->setViewPortRatio(width(),height());
    ctrl->draw();
    update();
}

void GLWidget::resizeGL(int w, int h){
    makeCurrent();
    ctrl->setViewPortRatio(w,h);
}

void GLWidget::mousePressEvent(QMouseEvent *event){
    mLastPos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event){
    event->ignore();
    if (event->buttons() & Qt::MidButton) {
        //按下了鼠标中键
        if(QApplication::keyboardModifiers() == Qt::ControlModifier){
            //按下了control键
            QPoint mPos = event->pos();
            glm::vec4 viewPort(0,0,width(),height());
            ctrl->processTranslation(mPos, mLastPos, viewPort);
            update();
            mLastPos = mPos;
        }
        else{
            QPoint mPos = event->pos();
            float deltaX = mPos.x() - mLastPos.x();
            float deltaY = mPos.y() - mLastPos.y();
            glm::vec4 viewPort(0,0,width(),height());
            ctrl->processRotation(mPos, mLastPos, viewPort);
            update();
            mLastPos = mPos;
        }
    }
}

void GLWidget::wheelEvent(QWheelEvent *event){
   auto degree = event->angleDelta();
   ctrl->processScroll(degree.y());
   update();
}

void GLWidget::bindController(Controller *c){
    ctrl = c;
    c->bindGLWidget(this);
}



QOpenGLContext* GLWidget::getGLContext(){
    makeCurrent();
    return QOpenGLContext::currentContext();
}

void GLWidget::setConnector(GuiConnector *c){
    connector = c;
}

void GLWidget::setClickable(bool flag){
    if(flag){
        removeEventFilter(this);
        installEventFilter(this);
    }
    else{
        removeEventFilter(this);
    }
}

bool GLWidget::eventFilter(QObject *watched, QEvent *event){
    if(watched == this){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent* mouseEvent = dynamic_cast<QMouseEvent*>(event);
            //construct a line
            if(mouseEvent->button() == Qt::LeftButton){
                processIntersection(mouseEvent);
                return true;
            }
            else{
                return QOpenGLWidget::eventFilter(watched, event);
            }
        }
        else{
            return QOpenGLWidget::eventFilter(watched, event);
        }
    }
    else{
        return QOpenGLWidget::eventFilter(watched, event);
    }
}

void GLWidget::processIntersection(QMouseEvent *event){
    int x = event->x();
    int y = event->y();
    glm::vec3 nearScreen{x, y, 0};
    glm::vec3 farScreen(x, y, 1);
    glm::vec3 nearPoint = glm::unProject(nearScreen, camera->viewMatrix()*glm::mat4(1.0f),
                                         camera->perspectiveMatrix(), getGLViewport());
    glm::vec3 farPoint = glm::unProject(farScreen, camera->viewMatrix()*glm::mat4(1.0f),
                                         camera->perspectiveMatrix(), getGLViewport());
    ctrl->processIntersectionPoint(nearPoint, farPoint-nearPoint);
}

glm::vec4 GLWidget::getGLViewport(){
    return glm::vec4(0,0,width(),height());
}
