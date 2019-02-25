#include "GuiConnector.h"
#include "mainwindow.h"
#include "CentralWidget.h"
#include "TabBackground.h"
#include "NewCurveTab.h"
#include <QTextEdit>

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
