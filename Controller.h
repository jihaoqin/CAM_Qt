#pragma once
#include "Data.h"
#include <string>
#include <QOpenGLContext>
#include "GLProgram.h"
#include <QOpenGLFunctions_4_3_Core>
#include <memory>
class Controller
{
public:
    Controller();
    BoundingBox updateBoundingBox();
    void save();
    void bindData(std::shared_ptr<Data>);
    void addModel(std::string);
    void addTee(QOpenGLContext*, float ,float ,float ,float);
    void addLine();
    void bindGL(QOpenGLContext*, std::shared_ptr<GLBinder>);
    void draw(std::shared_ptr<GLProgram>);
    bool hasTee();
    void setViewPortRatio(int w, int h);
    void processTranslation(QPoint mPos, QPoint mLastPos, glm::vec4 viewPort);
    void processRotation(QPoint mPos, QPoint mLastPos, glm::vec4 viewPort);
    void processScroll(double yOffset);
    std::shared_ptr<Camera2> getCamera();
private:
    std::shared_ptr<Data> data;
};

