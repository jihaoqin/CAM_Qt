#pragma once
#include <string>
#include <QString>
#include <QOpenGLContext>
#include "GLProgram.h"
#include <QOpenGLFunctions_4_3_Core>
#include <memory>
#include "DataObject.h"
class GLWidget;
class MainWindow;
class Data;
class Camera2;
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
    void addPoint(glm::vec3);
    void draw();
    void drawDataObject(std::shared_ptr<DataObject>);
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
