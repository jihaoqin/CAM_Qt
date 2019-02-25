#pragma once
#include "Data.h"
#include <string>
#include <QString>
#include <QOpenGLContext>
#include "GLProgram.h"
#include <QOpenGLFunctions_4_3_Core>
#include <memory>
class GLWidget;
class MainWindow;
class Controller
{
public:
    Controller();
    void bindMainWindow(MainWindow* m);
    BoundingBox updateBoundingBox();
    void save(QString);
    void bindData(std::shared_ptr<Data>);
    void bindGLWidget(GLWidget*);
    void addTee(float ,float ,float ,float);
    void addLine();
    void addCylinder(glm::vec3 begin, glm::vec3 end, float r);
    void addRing(double R_, double r_, double angle_ , glm::vec3 anchor_, glm::vec3 zdir_, glm::vec3 xdir_);
    void bindGL(QOpenGLContext*, std::shared_ptr<GLBinder>);
    void draw(std::shared_ptr<GLProgram>);
    bool hasTee();
    void setViewPortRatio(int w, int h);
    void processTranslation(QPoint mPos, QPoint mLastPos, glm::vec4 viewPort);
    void processRotation(QPoint mPos, QPoint mLastPos, glm::vec4 viewPort);
    void processScroll(double yOffset);
    void clearData();
    std::shared_ptr<Camera2> getCamera();
    bool getEmpty();
    bool getChanged();
    void addIntersectionPoint(glm::vec3 begin, glm::vec3 dir);
    void processIntersectionPoint(glm::vec3 begin, glm::vec3 dir);
private:
    std::shared_ptr<Data> data;
    GLWidget* widget;
    MainWindow* mainWindow;
};
