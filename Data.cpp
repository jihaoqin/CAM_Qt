#include "Data.h"
using std::shared_ptr;
Data::Data():box()
{
    camera = std::make_shared<Camera2>(BoundingBox());
}


void Data::updateBoundingBox(){
   vector<BoundingBox> boxVec;
   if(tee){
        boxVec.push_back(tee->boundingBox());
   }
   box = BoundingBox::OrBox(boxVec);
}

void Data::addTee(std::shared_ptr<Tee> t){
     tee = t;
     updateBoundingBox();
     camera->bindBoundingBox(box);
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
}

void Data::processRotation(QPoint mPos, QPoint mLastPos, glm::vec4 viewPort){
    camera->processRotation(mPos, mLastPos, viewPort);
}

void Data::processScroll(double yOffset){
    camera->processScroll(yOffset);
}
