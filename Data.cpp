#include "Data.h"
#include <fstream>
#include "Cylinder.h"
#include "Ring.h"
#include "Point.h"
#include "Node.h"
#include "Curve.h"
#include "Band.h"
#include "GuiConnector.h"
#include <QMutexLocker>
using std::shared_ptr;
using std::make_shared;
Data::Data():box(), state(), idGenerator(), root(nullptr), connector(nullptr)
  , mtx(QMutex::Recursive)
{
    camera = std::make_shared<Camera2>(BoundingBox());
}


void Data::updateBoundingBox(){
    QMutexLocker locker(&mtx);
    vector<BoundingBox> boxVec;
    box = root->boudingBoxUnion();
}

void Data::addTee(std::shared_ptr<Tee> t){
    QMutexLocker locker(&mtx);
    root = std::make_shared<Node>(t);
    updateBoundingBox();
    camera->bindBoundingBox(box);
    state.setEmpty(false);
    state.setChanged(true);
}

void Data::addPoint(shared_ptr<Point> p){
    QMutexLocker locker(&mtx);
    root->addChild(make_shared<Node>(p));
    updateBoundingBox();
    state.setEmpty(false);
    state.setChanged(true);
}

void Data::addCurve(std::shared_ptr<Curve> curve){
    QMutexLocker locker(&mtx);
    root->addChild(make_shared<Node>(curve));
    updateBoundingBox();
    state.setEmpty(false);
    state.setChanged(true);
}
void Data::addBand(std::shared_ptr<Band> b){
    QMutexLocker locker(&mtx);
    root->addChild(make_shared<Node>(b));
    connector->updateModel();
    updateBoundingBox();
    state.setEmpty(false);
    state.setChanged(true);
}

void Data::deleteBand(QString id){
    QMutexLocker locker(&mtx);
    if(!id.contains("band")){
        return ;
    }
    DataObjectPtr obj = root->findObjectId(id.toLatin1().data());
    if(obj == nullptr){
        return ;
    }
    QStringVec fatherIds = obj->fatherId();
    QStringVec allIds;
    for(auto id:fatherIds){
        allIds.push_back(id);
    }
    for(auto i:fatherIds){
        DataObjectPtr obj = root->findObjectId(i.toLatin1().data());
        if(obj != nullptr){
            QStringVec childIds = obj->childId();
            for(auto id:childIds){
                allIds.push_back(id);
            }
        }
    }
    for(auto id:allIds){
        root->deleteChild(id.toLatin1().data());
    }
    root->deleteChild(id.toLatin1().data());
    connector->updateModel();
    updateBoundingBox();
    state.setEmpty(false);
    state.setChanged(true);
}

bool Data::hasTee(){
    QMutexLocker locker(&mtx);
    if(root!= nullptr && root->getData() != nullptr){
        return true;
    }
    else{
        return false;
    }
}

void Data::setViewPortRatio(int w, int h){
    QMutexLocker locker(&mtx);
    camera->viewPortRatio(w, h);
}

void Data::processTranslation(QPoint mPos, QPoint mLastPos, glm::vec4 viewPort){
    QMutexLocker locker(&mtx);
    camera->processTranslation(mPos, mLastPos, viewPort);
    state.setChanged(true);
}

void Data::processRotation(QPoint mPos, QPoint mLastPos, glm::vec4 viewPort){
    QMutexLocker locker(&mtx);
    camera->processRotation(mPos, mLastPos, viewPort);
    state.setChanged(true);
}

void Data::processScroll(double yOffset){
    QMutexLocker locker(&mtx);
    camera->processScroll(yOffset);
    state.setChanged(true);
}

void Data::clear(){
    QMutexLocker locker(&mtx);
    if(state.getEmpty() == false){
        state.setChanged(true);
    }
    else{
        state.setChanged(false);
    }
    state.setEmpty(true);

    updateBoundingBox();
}

bool Data::getEmpty(){
    QMutexLocker locker(&mtx);
    return state.getEmpty();
}

bool Data::getChanged(){
    QMutexLocker locker(&mtx);
    return state.getChanged();
}


void Data::bindConnector(GuiConnector *c){
    QMutexLocker locker(&mtx);
    connector = c;
}

std::shared_ptr<Node> Data::getNodeRoot(){
    QMutexLocker locker(&mtx);
    return root;
}
