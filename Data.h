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
#include <DataState.h>
class Cylinder;
class Ring;
class Data
{
public:
    friend class Controller;
public:
    Data();
    void addTee(std::shared_ptr<Tee>);
    void addCylinder(std::shared_ptr<Cylinder>);
    void addRing(std::shared_ptr<Ring>);
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
    std::shared_ptr<Tee> tee;
    std::shared_ptr<Cylinder> cyliner;
    std::shared_ptr<Ring> ring;
    BoundingBox box;
    DataState state;
    void updateBoundingBox();

    //serialization
    friend class boost::serialization::access;
    template<typename Archive>
    void serialize(Archive& ar, const unsigned int version){
        ar& camera & tee & box;
    }

};
