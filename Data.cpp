#include "Data.h"
#include <fstream>
#include "Cylinder.h"
#include "Ring.h"
#include "Point.h"
#include "Node.h"
#include "Curve.h"
#include "Band.h"
#include "GuiConnector.h"
#include "HangingBandSet.h"
#include <QMutexLocker>
using std::shared_ptr;
using std::make_shared;
Data::Data():box(), state(), idGenerator(), root(nullptr), connector(nullptr)
  , mtx(QMutex::Recursive), axiss(5), m_bandWidth(2)
{
    camera = std::make_shared<Camera2>(BoundingBox());
}


void Data::updateBoundingBox(){
    vector<BoundingBox> boxVec;
    box = root->boudingBoxUnion();
}

void Data::addTee(std::shared_ptr<Tee> t){
    root = std::make_shared<Node>(t);
    updateBoundingBox();
    camera->bindBoundingBox(box);
    state.setEmpty(false);
    state.setChanged(true);
}

void Data::addPoint(shared_ptr<Point> p){
    root->addChild(make_shared<Node>(p));
    updateBoundingBox();
    state.setEmpty(false);
    state.setChanged(true);
}

void Data::addCurve(std::shared_ptr<Curve> curve){
    root->addChild(make_shared<Node>(curve));
    updateBoundingBox();
    state.setEmpty(false);
    state.setChanged(true);
}
void Data::addBand(std::shared_ptr<Band> b){
    root->addChild(make_shared<Node>(b));
    //connector->updateModel();
    updateBoundingBox();
    state.setEmpty(false);
    state.setChanged(true);
}

void Data::addHang(std::shared_ptr<HangingBandSet> h){
    root->addChild(make_shared<Node>(h));
    updateBoundingBox();
    state.setEmpty(false);
    state.setChanged(true);
}

void Data::deleteBand(QString id){
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
    if(root!= nullptr && root->getData() != nullptr){
        return true;
    }
    else{
        return false;
    }
}

void Data::setViewPortRatio(int w, int h){
    camera->viewPortRatio(w, h);
}

void Data::processTranslation(QPoint mPos, QPoint mLastPos, glm::vec4 viewPort){
    camera->processTranslation(mPos, mLastPos, viewPort);
    state.setChanged(true);
}

void Data::processRotation(QPoint mPos, QPoint mLastPos, glm::vec4 viewPort){
    camera->processRotation(mPos, mLastPos, viewPort);
    state.setChanged(true);
}

void Data::processScroll(double yOffset){
    camera->processScroll(yOffset);
    state.setChanged(true);
}

void Data::clear(){
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
    return state.getEmpty();
}

bool Data::getChanged(){
    return state.getChanged();
}


void Data::bindConnector(GuiConnector *c){
    connector = c;
}

std::shared_ptr<Node> Data::getNodeRoot(){
    return root;
}

QMutex* Data::getMutex(){
    return &mtx;
}


AxisIni& Data::getAxissIni(){
    return axiss;
}


EnvelopData& Data::getEnvelopeIni(){
    return envelop;
}


float Data::bandWidth(){
    return m_bandWidth;
}

void Data::bandWidth(float w){
    m_bandWidth = w;
}


void Data::serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) const{
    writer.StartObject();
    writer.String("AxisIni");
    axiss.serialize(writer);
    writer.String("EnvelopData");
    envelop.serialize(writer);
    writer.String("width");
    writer.Double(m_bandWidth);
    writer.String("idGenerator");
    idGenerator.serialize(writer);
    writer.EndObject();
}
