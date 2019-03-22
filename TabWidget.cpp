#include "TabWidget.h"
#include <QDebug>
#include "TabBackground.h"
#include <QLabel>
#include "NewCurveTab.h"
#include "GuiConnector.h"

TabWidget::TabWidget(QWidget* parent)
    :QTabWidget(parent),first(nullptr)
{
    setMouseTracking(true);
    setStyleSheet("background-color:rgb(255,255,255)");
    //setMinimumWidth(200);
    first = new TabBackground(this);
    first->setMouseTracking(true);
    addTab(first, "first");

    operation = new TabBackground(this);
    addTab(operation, "op");

}

void TabWidget::showNewCurveTab(){
    NewCurveTab* newCurve = new NewCurveTab(this);
    newCurve->setConnector(connector);
    operation->setWidget(newCurve);
    newCurve->setTabBack(operation);
    setCurrentIndex(1);
}


bool TabWidget::isOperating(){
    return operation->isSet();
}

void TabWidget::setConnector(GuiConnector *c){
    connector = c;
    first->setConnector(c);
    operation->setConnector(c);
}
