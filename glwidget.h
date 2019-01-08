#ifndef GLWIDGET_H
#define GLWIDGET_H
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLFunctions_4_3_Core>
#include <QOpenGLShaderProgram>
#include <QWheelEvent>
#include "Mesh.h"
#include "Model.h"
#include "Camera2.h"

class GLWidget : public  QOpenGLWidget, protected QOpenGLFunctions_4_3_Core
{
    Q_OBJECT
public:
    GLWidget(QWidget *parent = 0);
    void bindGL(Model&);
protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent* event)	override;
private:
    void setMat4(const char*, const glm::mat4);
    //model和camera的顺序很重要，camera依赖于model
    Model model;
    Camera2 camera;
    GLuint VAO, VBO;
    QOpenGLShaderProgram program;
    QOpenGLContext* context;
    QPoint mLastPos;
};

#endif // GLWIDGET_H
