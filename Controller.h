#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "Data.h"
#include <string>
#include <QOpenGLContext>

class Controller
{
public:
    Controller();
    void addModel(std::string);
    void addLine();
    void bindGL(QOpenGLContext*, std::shared_ptr<GLBinder>);
    void draw();
private:
    Data *data;
};

#endif // CONTROLLER_H
