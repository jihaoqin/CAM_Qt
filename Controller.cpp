#include "Controller.h"
#include <memory>
using namespace  std;
Controller::Controller()
{

}


void Controller::bindGL(QOpenGLContext *c, shared_ptr<GLBinder> obj){
   obj->bindGL(c);
}

void Controller::draw(std::shared_ptr<GLProgram> program){
    data->camera->setUniform(program);
    if(data->tee){
        data->tee->draw(program);
    }
}

std::shared_ptr<Camera2> Controller::getCamera(){
    return data->camera;
}
void Controller::bindData(std::shared_ptr<Data> d){
    data = d;
}

BoundingBox Controller::updateBoundingBox(){
   vector<BoundingBox> boxVec;
   if(data->tee){
        boxVec.push_back(data->tee->boundingBox());
   }
   data->box = BoundingBox::OrBox(boxVec);
   return data->box;
}

void Controller::addTee(QOpenGLContext* context, float mainLength, float branchLength, float R, float sideR){
    std::shared_ptr<Tee> t = std::make_shared<Tee>(mainLength, branchLength, R, sideR);
    t->bindGL(context);
    data->addTee(t);
}

bool Controller::hasTee(){
    return data->hasTee();
}

void Controller::setViewPortRatio(int w, int h){
    data->setViewPortRatio(w, h);
}

void Controller::processTranslation(QPoint mPos, QPoint mLastPos, glm::vec4 viewPort){
    data->processTranslation(mPos, mLastPos, viewPort);
}

void Controller::processRotation(QPoint mPos, QPoint mLastPos, glm::vec4 viewPort){
    data->processRotation(mPos, mLastPos, viewPort);
}

void Controller::processScroll(double yOffset){
    data->processScroll(yOffset);
}
