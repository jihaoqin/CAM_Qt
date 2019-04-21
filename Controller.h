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
#include "Band.h"
#include <set>
class GLWidget;
class MainWindow;
class Data;
class Point;
class PointSym;
class Camera2;
class Tee;
class GenCurveProgressDialog;
using TeePtr = std::shared_ptr<Tee>;
class CBandOnPoint;
using CBandPtr = std::shared_ptr<CBandOnPoint>;
class Controller
{
public:
    friend class NewCurveController;
    friend class GuiConnector;
    friend class OpenBandThread;
    friend class GenGeneralBandThread;
    friend class ClosePathThread;
    friend class AnimateController;
    Controller();
    std::shared_ptr<Data> getData();
    void bindMainWindow(MainWindow* m);
    BoundingBox updateBoundingBox();
    void bindData(std::shared_ptr<Data>);
    void bindGLWidget(GLWidget*);
    void addTee(float ,float ,float ,float);
    void setColor(QStringVec, Color);
    EndPtrVec allEnds();
    std::set<End> allEndsSet();
    QString addBandInTee(QString pId, float uAng);
    CBandPtr addCBandInPipe(QString pId, float uAng);
    void deleteBand(QString id);
    std::shared_ptr<Point> addPoint(HalfPoint);
    void draw();
    void drawDataObject(std::shared_ptr<DataObject>);
    bool hasTee();
    GuiConnector* getConnector();
    void genLeftCurve();
    void genUpCurve();
    void genRightCurve();
    void closePath();
    void genCylinderCurve(QString);
    void setViewPortRatio(int w, int h);
    void processTranslation(QPoint mPos, QPoint mLastPos, glm::vec4 viewPort);
    void processRotation(QPoint mPos, QPoint mLastPos, glm::vec4 viewPort);
    void processScroll(double yOffset);
    void clearData();
    std::shared_ptr<Camera2> getCamera();
    bool getEmpty();
    bool getChanged();
    QString addIntersectionPointInTee(glm::vec3 begin, glm::vec3 dir);
    QString addIntersectionPointInPipe(glm::vec3 begin, glm::vec3 dir);
    void processIntersectionPoint(glm::vec3 begin, glm::vec3 dir, glm::vec2 glXY);
    void clickOnPoint(QString, glm::vec2);
    void processIntersectionWhenRelease();
    void exitPick();
    void setPointPickState(QString, bool);
    void processMoveWhenMove(glm::vec3, glm::vec3);
    void processMoveInTee(Pos, Dir);
    void processMoveInPipe(Pos, Dir);
    std::vector<HalfPoint> intersectionPointInTee(glm::vec3, glm::vec3);
    std::vector<HalfPoint> intersectionPointInPipe(glm::vec3, glm::vec3);
    void updateCurve(QString, float, float);
    void saveBand(QString path);
    void openBand(QString path);
    void allBindGL();
    QOpenGLContext* getGLContext();
private:
    std::shared_ptr<Data> data;
    GLWidget* widget;
    MainWindow* mainWindow;
};
