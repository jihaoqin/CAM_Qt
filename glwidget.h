#ifndef GLWIDGET_H
#define GLWIDGET_H
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLFunctions_4_3_Core>
#include <QOpenGLShaderProgram>
#include "Mesh.h"
#include "Model.h"

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
private:
    void bindGL(Mesh&);
    Model model;
    const float point[9];
    GLuint VAO, VBO;
    QOpenGLShaderProgram program;
};

#endif // GLWIDGET_H
