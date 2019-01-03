#ifndef GLWIDGET_H
#define GLWIDGET_H
#include <QOpenGLWidget>
#include <QOpenGLFunctions>

class GLWidget : public  QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    GLWidget();
};

#endif // GLWIDGET_H
