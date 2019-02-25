#include "GuiConnector.h"
#include "mainwindow.h"
#include "CentralWidget.h"
#include "TabBackground.h"

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

bool GuiConnector::isPointTextEmpty(){
    if(mainWindow->widget->tabWidget->operation->isSet()){//如果是newCurveOperation
    }
}
