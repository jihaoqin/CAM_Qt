#include "GuiConnector.h"
#include "mainwindow.h"
#include "CentralWidget.h"
#include "TabBackground.h"
#include "NewCurveTab.h"
#include <QTextEdit>
#include "GLProgram.h"
#include <memory>

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

std::shared_ptr<GLProgram> GuiConnector::getMeshProgram(){
    GLWidget* glWidget = mainWindow->widget->glWidget;
    return glWidget->meshProgram;
}

std::shared_ptr<GLProgram> GuiConnector::getPointProgram(){
    GLWidget* glWidget = mainWindow->widget->glWidget;
    return glWidget->pointProgram;
}
