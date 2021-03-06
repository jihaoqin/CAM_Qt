#include "GuiConnector.h"
#include "mainwindow.h"
#include "CentralWidget.h"
#include "TabBackground.h"
#include "NewCurveTab.h"
#include <QTextEdit>
#include "GLProgram.h"
#include <memory>
#include "ObjTreeWidget.h"
#include "FromPipeCurveTab.h"

GuiConnector::GuiConnector()
    :mainWindow(nullptr)
{

}

void GuiConnector::setRoot(MainWindow *m){
    mainWindow = m;
}

void GuiConnector::setGLWidgetClickable(bool flag){
    GLWidget* glWidget = mainWindow->widget->glWidget;
    glWidget->setClickable(flag);
}

QString GuiConnector::getPointText(){
    TabBackground* op = mainWindow->widget->tabWidget->operation;
    if(op->isSet()){//如果是newCurveOperation
       NewCurveTab* newCurveTab = dynamic_cast<NewCurveTab*>(op->center);
       QString pointId = newCurveTab->pointText->toPlainText();
       return pointId;
    }
}
void GuiConnector::setPointText(QString s){
    TabBackground* op = mainWindow->widget->tabWidget->operation;
    if(op->isSet()){//如果是newCurveOperation
       NewCurveTab* newCurveTab = dynamic_cast<NewCurveTab*>(op->center);
       newCurveTab->pointText->setText(s);
    }
}

std::shared_ptr<GLProgram> GuiConnector::getMeshProgram(){
    GLWidget* glWidget = mainWindow->widget->glWidget;
    return glWidget->meshProgram;
}

std::shared_ptr<GLProgram> GuiConnector::getPointProgram(){
    GLWidget* glWidget = mainWindow->widget->glWidget;
    return glWidget->pointProgram;
}

NewCurveTab* GuiConnector::getNewCurveTabWidget(){
    TabBackground* op = mainWindow->widget->tabWidget->operation;
    if(op->isSet()){//如果是newCurveOperation
       NewCurveTab* newCurveTab = dynamic_cast<NewCurveTab*>(op->center);
       return newCurveTab;
    }
    else{
        return nullptr;
    }
}

FromPipeCurveTab* GuiConnector::getFromPipeCurveTab(){
    TabBackground* op = mainWindow->widget->tabWidget->operation;
    if(op->isSet()){//如果是newCurveOperation
       FromPipeCurveTab* pipeCurveTab = dynamic_cast<FromPipeCurveTab*>(op->center);
       return pipeCurveTab;
    }
    else{
        return nullptr;
    }
}

GLWidget* GuiConnector::getGLWidget(){
    GLWidget* glWidget = mainWindow->widget->glWidget;
    return glWidget;
}


void GuiConnector::updateCurve(QString id, float angle, float coe){
    mainWindow->ctrl->updateCurve(id, angle, coe);
}

Controller* GuiConnector::getCtrl(){
    return mainWindow->ctrl;
}

void GuiConnector::updateAction(){
    mainWindow->updateAction();
}

void GuiConnector::updateModel(){
    mainWindow->widget->tabWidget->tree->updateModel();
}

std::shared_ptr<Data> GuiConnector::getData(){
    return getCtrl()->data;
}

TabWidget* GuiConnector::getTabWidget(){
    return mainWindow->widget->tabWidget;
}


SimulationTab* GuiConnector::getSimTab(){
    return (SimulationTab*)mainWindow->widget->tabWidget->operation->center;
}
