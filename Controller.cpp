#include "Controller.h"
#include <memory>
#include "glwidget.h"
#include "mainwindow.h"
#include "GuiConnector.h"
#include "Cylinder.h"
#include "Ring.h"

using namespace  std;
Controller::Controller()
{

}


void Controller::bindGL(QOpenGLContext *c, shared_ptr<GLBinder> obj){
   obj->bindGL(c);
}

void Controller::draw(std::shared_ptr<GLProgram> program){
    data->camera->setUniform(program);
    if(data->getEmpty() == false){
        data->tee->draw(program);
        data->cyliner->draw(program);
        data->ring->draw(program);
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
   if(data->getEmpty() == false){
        boxVec.push_back(data->tee->boundingBox());
   }
   data->box = BoundingBox::OrBox(boxVec);
   return data->box;
}

void Controller::addTee(float mainLength, float branchLength, float R, float sideR){
    std::shared_ptr<Tee> t = std::make_shared<Tee>(mainLength, branchLength, R, sideR);
    QOpenGLContext* c = widget->getGLContext();
    t->bindGL(c);
    data->addTee(t);
    mainWindow->updateAction();
}

void Controller::addRing(double R_, double r_, double angle_, glm::vec3 anchor_, glm::vec3 zdir_, glm::vec3 xdir_){
    std::shared_ptr<Ring> ring = std::make_shared<Ring>(R_, r_, angle_, anchor_, zdir_, xdir_);
    QOpenGLContext* c = widget->getGLContext();
    ring->bindGL(c);
    data->addRing(ring);
    mainWindow->updateAction();
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

void Controller::save(QString savePath){
    data->save(savePath);
}

void Controller::clearData(){
    data->clear();
}

bool Controller::getEmpty(){
    return data->getEmpty();
}

bool Controller::getChanged(){
    return data->getChanged();
}

void Controller::bindGLWidget(GLWidget* w){
    widget = w;
}

void Controller::bindMainWindow(MainWindow* m){
    mainWindow = m;
}

void Controller::processIntersectionPoint(glm::vec3 begin, glm::vec3 dir){
    //判断是创建点，还是选择点
        //根据pointText内是否已有可视点，如果有可视点，就是选择点操作，如果无可视点就是创建点操作
    //如果是选择点操作
        //后面的点击操作只改变此点的位置，并不新创建点
    //如果是创建点操作
        //创建点
    QString pointId = mainWindow->connector->getPointText();
    if (pointId.isEmpty()){
        //创建点
        addIntersectionPoint(begin, dir);
    }
    else{
        //选择点
    }
}

void Controller::addIntersectionPoint(glm::vec3 begin, glm::vec3 dir){
    //TODO
}

void Controller::addCylinder(glm::vec3 begin, glm::vec3 end, float r){
    std::shared_ptr<Cylinder> cylinder = std::make_shared<Cylinder>(begin, end, r);
    QOpenGLContext* c = widget->getGLContext();
    cylinder->bindGL(c);
    data->addCylinder(cylinder);
    mainWindow->updateAction();
}
