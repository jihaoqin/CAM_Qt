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
#include "RingCurveAssist.h"
#include "Curve.h"
#include "RingCurve.h"

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
    std::shared_ptr<Tee> t = std::make_shared<Tee>(mainLength, branchLength, R, sideR, data->idGenerator);
    QOpenGLContext* c = widget->getGLContext();
    t->bindGL(c);
    data->addTee(t);
    t->setIdUsing(data->idGenerator);
    mainWindow->updateAction();
}


std::shared_ptr<Point> Controller::addPoint(HalfPoint p){
    QString id = data->idGenerator.getPointId();
    std::shared_ptr<Point> point = std::make_shared<Point>(p.pos, id.toLatin1().data());
    point->meshId(p.meshName.c_str());
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

void Controller::processIntersectionPoint(glm::vec3 begin, glm::vec3 dir, glm::vec2 glXY){
    //判断是创建点，还是选择点
        //根据pointText内是否已有可视点，如果有可视点，就是选择点操作，如果无可视点就是创建点操作
    //如果是选择点操作
        //后面的点击操作只改变此点的位置，并不新创建点
    //如果是创建点操作
        //创建点
    NewCurveTab* newCurveTab = mainWindow->connector->getNewCurveTabWidget();
    QString pointId = newCurveTab->getPointText();
    bool focusOnPointText = newCurveTab->isPointTextFocused();
    if(focusOnPointText == true){
        if(pointId.isEmpty()){
            //创建点
            QString id = addIntersectionPoint(begin, dir);
            newCurveTab->setPointText(id);
            float angle = newCurveTab->getWindingAngle()*utility::PI/180;
            QString curveId = addCurve(id,angle);
            newCurveTab->setCurveId(curveId);
        }
        else{
            //赋选中点picked
            clickOnPoint(pointId, glXY);
        }
    }
    else{
        return ;
    }
}

std::vector<HalfPoint> Controller::intersectionPointInTee(glm::vec3 begin, glm::vec3 dir){
    DataObjectPtr teeBase = data->root->findObjectId("tee");
    auto tee = dynamic_pointer_cast<Tee>(teeBase);
    if(tee == nullptr){
        return vector<HalfPoint>();
    }
    vector<HalfPoint> points = tee->intersectionPoints(begin, dir);
    if(points.size() == 0){
        return vector<HalfPoint>();
    }
    else{
        float dist = utility::length(points.at(0).pos - begin);
        int minIndex = 0;
        for(int i = 0; i < points.size(); i++){
            float l = utility::length(points.at(i).pos - begin);
            if(l < dist){
                dist = l;
                minIndex = i;
            }
        }
        return std::vector<HalfPoint>{points.at(minIndex)};
    }
}

QString Controller::addIntersectionPoint(glm::vec3 begin, glm::vec3 dir){
    auto pointVec = intersectionPointInTee(begin,dir);
    if(pointVec.size()==0){
        return QString();
    }
    else{
        auto pointPtr = addPoint(pointVec.at(0));
        return QString(pointPtr->getId());
    }
}

void Controller::drawDataObject(std::shared_ptr<DataObject> ob){
    assert(ob);
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


void Controller::clickOnPoint(QString id, glm::vec2 sample2D){
    auto root = data->root;
    if(root == nullptr){
        return ;
    }
    DataObjectPtr objectPtr = root->findObjectId(id.toLatin1().data());
    if(objectPtr == nullptr){
        return ;
    }
    auto pointPtr = dynamic_pointer_cast<Point>(objectPtr);
    glm::vec3 pos3D = pointPtr->getPos();
    GLWidget *glWidget = mainWindow->connector->getGLWidget();
    glm::vec2 pos2D = glWidget->spatialTo2D(pos3D);
    if(utility::length(sample2D - pos2D) < 4){
        pointPtr->picked = true;
    }
}

void Controller::processIntersectionWhenRelease(){
    NewCurveTab* newCurveTab = mainWindow->connector->getNewCurveTabWidget();
    bool focusOnPointText = newCurveTab->isPointTextFocused();
    if(focusOnPointText){
        QString pointId = newCurveTab->getPointText();
        setPointPickState(pointId, false);
    }
    else{
        return;
    }
}

void Controller::exitPick(){
    processIntersectionWhenRelease();
}

void Controller::setPointPickState(QString id, bool state){
    if(id.isEmpty()){
        return;
    }
    DataObjectPtr ptr = data->root->findObjectId(id.toLatin1().data());
    if(ptr){
        auto pointPtr = dynamic_pointer_cast<Point>(ptr);
        pointPtr->picked = state;
    }
}

void Controller::processMoveWhenMove(glm::vec3 begin, glm::vec3 dir){
    NewCurveTab* newCurveTab = mainWindow->connector->getNewCurveTabWidget();
    bool focusOnPointText = newCurveTab->isPointTextFocused();
    if(focusOnPointText){
        QString pointId = newCurveTab->getPointText();
        DataObjectPtr ptr = data->root->findObjectId(pointId.toLatin1().data());
        if(ptr == nullptr){
            return;
        }
        else{
            auto pointPtr = dynamic_pointer_cast<Point>(ptr);
            if(pointPtr->picked == true){
                auto pointVec = intersectionPointInTee(begin,dir);
                if(pointVec.size()==0){
                    return;
                }
                glm::vec3 pos = pointVec.at(0).pos;
                pointPtr->setPos(pos);
                GLWidget * gl = mainWindow->connector->getGLWidget();
                glm::vec2 p2d = gl->spatialTo2D(pos);
                glm::vec4 viewport = gl->getGLViewport();
            }
        }
    }
    else{
        return;
    }
}

QString Controller::addCurve(QString pId, float uAng){
    if(!pId.contains("point")){
        return QString("");
    }
    auto ptr = data->root->findObjectId(pId.toLatin1().data());
    if(ptr == nullptr){
        return QString("");
    }
    auto pointPtr = std::dynamic_pointer_cast<Point>(ptr);
    auto teeFa = data->root->findObjectId("tee");
    auto teePtr = std::dynamic_pointer_cast<Tee>(teeFa);
    QString meshId(pointPtr->meshId());
    if(meshId.contains("ring")){
        Ring* r = teePtr->getRing(meshId);
        QString id = data->idGenerator.getCurveId();
        auto curve = std::make_shared<RingCurve>(pointPtr,uAng,0.1,id.toLatin1().data(), r);
        pointPtr->addChild(curve);
        QOpenGLContext* gl = widget->getGLContext();
        curve->bindGL(gl);
        data->addCurve(curve);
        mainWindow->updateAction();
        return id;
    }
}

void Controller::updateCurve(QString id, float angle){
    DataObjectPtr ptr = data->root->findObjectId(id.toLatin1().data());
    assert(ptr);
    QString ptrId = QString(ptr->getId());
    if(!ptrId.contains("curve")){
        return;
    }
    auto curve = std::dynamic_pointer_cast<Curve>(ptr);
    if(curve->type == Curve::Type::general){
        auto ringcurve = std::dynamic_pointer_cast<RingCurve>(curve);
        ringcurve->setWindingAngle(angle);
    }
}
