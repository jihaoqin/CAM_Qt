#ifndef GLWIDGET_H
#define GLWIDGET_H
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLFunctions_4_3_Core>
#include "GLProgram.h"
#include <QWheelEvent>
#include <memory>
#include "Camera2.h"
#include "Controller.h"

class GuiConnector;

class GLWidget : public  QOpenGLWidget, protected QOpenGLFunctions_4_3_Core
{
    Q_OBJECT
public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();
    void bindController(Controller*);
    QOpenGLContext* getGLContext();
    void setConnector(GuiConnector*);
    void setClickable(bool);
    glm::vec2 spatialTo2D(glm::vec3);
    glm::vec3 get3DPoint(glm::vec3);
    glm::vec4 getGLViewport();
    friend class GuiConnector;
protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent* event)	override;
    bool eventFilter(QObject* watched, QEvent* event) override;
    void processIntersectionWhenPress(QMouseEvent* event);
    void processMoveWhenMove(QMouseEvent* event);
private:
    Controller* ctrl;
    //model和camera的顺序很重要，camera依赖于model
    std::shared_ptr<Camera2> camera;
    std::shared_ptr<GLProgram> meshProgram;
    std::shared_ptr<GLProgram> pointProgram;
    QOpenGLContext* context;
    QPoint mLastPos;
    GuiConnector* connector;
private:
};

#endif // GLWIDGET_H
