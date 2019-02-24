#include "GuiConnector.h"
#include "mainwindow.h"
#include "CentralWidget.h"

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
