#pragma once
#include "Model.h"
#include "config.h"
#include <vector>
#include "Line.h"
#include <memory>
#include "Camera2.h"
#include "Tee.h"
#include <QString>
#include "DataState.h"
#include "IdGenerator.h"
#include <QMutex>
class Cylinder;
class Ring;
class Point;
class Node;
class Curve;
class Band;
class GuiConnector;
class HangingBandSet;
class Data
{
public:
    friend class Controller;
    friend class NewCurveController;
    friend class FromPipeCurveController;
    friend class OpenBandThread;
    friend class GenGeneralBandThread;
    friend class ClosePathThread;
public:
    Data();
    void addTee(std::shared_ptr<Tee>);
    void addPoint(std::shared_ptr<Point>);
    void addCurve(std::shared_ptr<Curve>);
    void addBand(std::shared_ptr<Band>);
    void addHang(std::shared_ptr<HangingBandSet>);
    void deleteBand(QString id);
    bool hasTee();
    void setViewPortRatio(int w, int h);
    void processTranslation(QPoint mPos, QPoint mLastPos, glm::vec4 viewPort);
    void processRotation(QPoint mPos, QPoint mLastPos, glm::vec4 viewPort);
    void processScroll(double yOffset);
    void clear();
    bool getChanged();
    bool getEmpty();
    void bindConnector(GuiConnector*);
    QMutex* getMutex();
    std::shared_ptr<Node> getNodeRoot();
private:
    QMutex mtx;
    std::shared_ptr<Camera2> camera;
    std::shared_ptr<Node> root;
    BoundingBox box;
    DataState state;
    IdGenerator idGenerator;
    GuiConnector* connector;
    void updateBoundingBox();

};
