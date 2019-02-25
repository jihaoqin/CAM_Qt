#include "Controller.h"
#include <memory>
#include "glwidget.h"
#include "mainwindow.h"

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
/*
void Controller::addTee(QOpenGLContext* context, float mainLength, float branchLength, float R, float sideR){
    std::shared_ptr<Tee> t = std::make_shared<Tee>(mainLength, branchLength, R, sideR);
    t->bindGL(c);
    data->addTee(t);
}
*/

void Controller::addTee(float mainLength, float branchLength, float R, float sideR){
    std::shared_ptr<Tee> t = std::make_shared<Tee>(mainLength, branchLength, R, sideR);
    QOpenGLContext* c = widget->getGLContext();
    t->bindGL(c);
    data->addTee(t);
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
    if (mainWindow->connector->isPointTextEmpty()){
        //创建点
    }
    else{
        //选择点
    }
}

void Controller::addIntersectionPoint(glm::vec3 begin, glm::vec3 dir){
}
