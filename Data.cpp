#include "Data.h"
#include <fstream>
#include "Cylinder.h"
#include "Ring.h"
#include "Point.h"
#include "Node.h"
using std::shared_ptr;
Data::Data():box(), /*tee(nullptr),*/ state(), idGenerator(), root(nullptr)
{
    camera = std::make_shared<Camera2>(BoundingBox());
}


void Data::updateBoundingBox(){
   vector<BoundingBox> boxVec;
   解决树的遍历问题;
   if(tee){
        boxVec.push_back(tee->boundingBox());
   }
   box = BoundingBox::OrBox(boxVec);
}

void Data::addTee(std::shared_ptr<Tee> t){
    //tee = t;
    root = std::make_shared<Node>(t);
    updateBoundingBox();
    camera->bindBoundingBox(box);
    state.setEmpty(false);
    state.setChanged(true);
}

void Data::addPoint(shared_ptr<Point> p){
    //point = p;
    updateBoundingBox();
    camera->bindBoundingBox(box);
    state.setEmpty(false);
    state.setChanged(true);
}

bool Data::hasTee(){
    if(tee){
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

void Data::save(QString savePath){
    std::string s = savePath.toStdString();
    std::ofstream oStream(s.c_str());
    boost::archive::text_oarchive oa(oStream);
    oa << this;
    oStream.close();

    state.setChanged(false);
}

void Data::clear(){
    if(state.getEmpty() == false){
        state.setChanged(true);
    }
    else{
        state.setChanged(false);
    }
    state.setEmpty(true);

    tee = nullptr;
    updateBoundingBox();
}

bool Data::getEmpty(){
    return state.getEmpty();
}

bool Data::getChanged(){
    return state.getChanged();
}
