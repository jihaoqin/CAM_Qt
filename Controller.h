#pragma once
#include <string>
#include <QString>
#include <QOpenGLContext>
#include "GLProgram.h"
#include <QOpenGLFunctions_4_3_Core>
#include <memory>
#include "DataObject.h"
#include "HalfPoint.h"
#include "Color.h"
class GLWidget;
class MainWindow;
class Data;
class Point;
class PointSym;
class Camera2;
class Tee;
using TeePtr = std::shared_ptr<Tee>;
class Controller
{
public:
    friend class NewCurveController;
    friend class GuiConnector;
    Controller();
    void bindMainWindow(MainWindow* m);
    BoundingBox updateBoundingBox();
    void save(QString);
    void bindData(std::shared_ptr<Data>);
    void bindGLWidget(GLWidget*);
    void addTee(float ,float ,float ,float);
    void setColor(QStringVec, Color);
    QString addCurve(QString pId, float uAng);
    void deleteBand(QString id);
    std::shared_ptr<Point> addPoint(HalfPoint);
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
    QString addIntersectionPoint(glm::vec3 begin, glm::vec3 dir);
    void processIntersectionPoint(glm::vec3 begin, glm::vec3 dir, glm::vec2 glXY);
    void clickOnPoint(QString, glm::vec2);
    void processIntersectionWhenRelease();
    void exitPick();
    void setPointPickState(QString, bool);
    void processMoveWhenMove(glm::vec3, glm::vec3);
    std::vector<HalfPoint> intersectionPointInTee(glm::vec3, glm::vec3);
    void updateCurve(QString, float, float);
    void saveBand(QString path);
    void openBand(QString path);
private:
    std::shared_ptr<Data> data;
    GLWidget* widget;
    MainWindow* mainWindow;
};
