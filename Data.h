#pragma once
#include "ModelReader.h"
#include "config.h"
#include <vector>
#include <memory>
#include "Camera2.h"
#include "Tee.h"
#include <QString>
#include "DataState.h"
#include "IdGenerator.h"
#include "AxisIni.h"
#include <QMutex>
#include "EnvelopData.h"
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
    AxisIni& getAxissIni();
    QMutex* getMutex();
    float bandWidth();
    void bandWidth(float);
    std::shared_ptr<Node> getNodeRoot();
    EnvelopData& getEnvelopeIni();
    void serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const;
private:
    QMutex mtx;
    std::shared_ptr<Camera2> camera;
    std::shared_ptr<Node> root;
    BoundingBox box;
    DataState state;
    AxisIni axiss;
    EnvelopData envelop;
    float m_bandWidth;
    IdGenerator idGenerator;
    GuiConnector* connector;
    void updateBoundingBox();

};
