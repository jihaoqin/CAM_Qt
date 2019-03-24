#pragma once
#include "Model.h"
#include "config.h"
#include <vector>
#include "Line.h"
#include <memory>
#include "Camera2.h"
#include "Tee.h"
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <QString>
#include "DataState.h"
#include "IdGenerator.h"
class Cylinder;
class Ring;
class Point;
class Node;
class Curve;
class Band;
class Data
{
public:
    friend class Controller;
    friend class NewCurveController;
public:
    Data();
    void addTee(std::shared_ptr<Tee>);
    void addPoint(std::shared_ptr<Point>);
    void addCurve(std::shared_ptr<Curve>);
    void addBand(std::shared_ptr<Band>);
    bool hasTee();
    void setViewPortRatio(int w, int h);
    void processTranslation(QPoint mPos, QPoint mLastPos, glm::vec4 viewPort);
    void processRotation(QPoint mPos, QPoint mLastPos, glm::vec4 viewPort);
    void processScroll(double yOffset);
    void save(QString);
    void clear();
    bool getChanged();
    bool getEmpty();
private:
    std::shared_ptr<Camera2> camera;
    //std::shared_ptr<Tee> tee;
    //std::shared_ptr<Point> point;
    std::shared_ptr<Node> root;
    BoundingBox box;
    DataState state;
    IdGenerator idGenerator;
    void updateBoundingBox();

    //serialization
    friend class boost::serialization::access;
    template<typename Archive>
    void serialize(Archive& ar, const unsigned int version){
        ar& camera & box;
    }

};
