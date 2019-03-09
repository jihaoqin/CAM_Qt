#include "Controller.h"
#include <memory>
#include "glwidget.h"
#include "mainwindow.h"
#include "GuiConnector.h"
#include "Cylinder.h"
#include "Ring.h"
#include "Point.h"
#include "Data.h"
#include "Node.h"
#include "NewCurveTab.h"

using namespace  std;
Controller::Controller()
{

}



void Controller::draw(){
    NodePtr root = data->root;
    if(root != nullptr){
        auto drawFunc = [=](DataObjectPtr p)->void{ if(p != nullptr){drawDataObject(p);}};
        root->dataOperation(drawFunc);
    }
}

std::shared_ptr<Camera2> Controller::getCamera(){
    return data->camera;
}
void Controller::bindData(std::shared_ptr<Data> d){
    data = d;
}

BoundingBox Controller::updateBoundingBox(){
    data->updateBoundingBox();
    return data->box;
}

void Controller::addTee(float mainLength, float branchLength, float R, float sideR){
    std::shared_ptr<Tee> t = std::make_shared<Tee>(mainLength, branchLength, R, sideR);
    QOpenGLContext* c = widget->getGLContext();
    t->bindGL(c);
    data->addTee(t);
    mainWindow->updateAction();
}


std::shared_ptr<Point> Controller::addPoint(glm::vec3 p){
    QString id = data->idGenerator.getPointId();
    std::shared_ptr<Point> point = std::make_shared<Point>(p, id.toLatin1().data());
    QOpenGLContext* c = widget->getGLContext();
    point->bindGL(c);
    data->addPoint(point);
    mainWindow->updateAction();
    return point;
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
    NewCurveTab* newCurveTab = mainWindow->connector->getNewCurveTabWidget();
    QString pointId = newCurveTab->getPointText();
    QString dirId = newCurveTab->getDirText();
    bool focusOnPointText = newCurveTab->isPointTextFocused();
    bool focusOnDirText = newCurveTab->isDirTextFocused();
    if (pointId.isEmpty()){
        //创建点
        QString id = addIntersectionPoint(begin, dir);
        //mainWindow->connector->setPointText(id);
    }
    else{
        //选择点
    }
}

QString Controller::addIntersectionPoint(glm::vec3 begin, glm::vec3 dir){
    DataObjectPtr teeBase = data->root->findObjectId("tee");
    auto tee = dynamic_pointer_cast<Tee>(teeBase);
    if(tee == nullptr){
        return QString();
    }
    vector<glm::vec3> points = tee->intersectionPoints(begin, dir);
    if(points.size() == 0){
        return QString();
    }
    else{
        float dist = utility::length(points.at(0) - begin);
        int minIndex = 0;
        for(int i = 0; i < points.size(); i++){
            float l = utility::length(points.at(i) - begin);
            if(l < dist){
                dist = l;
                minIndex = i;
            }
        }
        auto pointPtr = addPoint(points.at(minIndex));
        return QString(pointPtr->getId());
    }
}

void Controller::drawDataObject(std::shared_ptr<DataObject> ob){
    QString id(ob->getId());
    std::shared_ptr<GLProgram> p;
    if(id.contains("point")){
        p = mainWindow->connector->getPointProgram();
    }
    else if(id.contains("line")){
        //to do
    }
    else if(id.contains("direction")){
        //to do
    }
    else{
        p = mainWindow->connector->getMeshProgram();
    }
    p->bind();
    data->camera->setUniform(p);
    ob->draw(p);
}
