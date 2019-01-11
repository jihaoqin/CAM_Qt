#ifndef GLWIDGET_H
#define GLWIDGET_H
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLFunctions_4_3_Core>
#include "GLProgram.h"
#include <QWheelEvent>
#include <memory>
#include "Mesh.h"
#include "Model.h"
#include "Camera2.h"
#include "Line.h"
#include "Controller.h"
class GLWidget : public  QOpenGLWidget, protected QOpenGLFunctions_4_3_Core
{
    Q_OBJECT
public:
    GLWidget(QWidget *parent = 0);
protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent* event)	override;
private:
    Controller* ctrl;
    //model和camera的顺序很重要，camera依赖于model
    Camera2 camera;
    std::shared_ptr<GLProgram> program;
    QOpenGLContext* context;
    QPoint mLastPos;
};

#endif // GLWIDGET_H
