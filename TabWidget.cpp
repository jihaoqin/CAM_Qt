#include "TabWidget.h"
#include <QDebug>
#include "TabBackground.h"
#include <QLabel>
#include "NewCurveTab.h"
#include "GuiConnector.h"
#include "ObjTreeWidget.h"
#include "SimulationTab.h"
#include "FromPipeCurveTab.h"
#include "MoveDataTab.h"

TabWidget::TabWidget(QWidget* parent)
    :QTabWidget(parent),tree(nullptr), job("")
{
    setMouseTracking(true);
    setStyleSheet("background-color:rgb(255,255,255)");
    tree = new ObjTreeWidget(this);
    tree->setMouseTracking(true);
    addTab(tree, "Tree");
    operation = new TabBackground(this);
    addTab(operation, "Op");

}

void TabWidget::showNewCurveTab(){
    NewCurveTab* newCurve = new NewCurveTab(this);
    newCurve->setConnector(connector);
    operation->setWidget(newCurve);
    newCurve->setTabBack(operation);
    setCurrentIndex(1);
    job = "newCurveTab";
}

void TabWidget::showSimulationTab(){
    SimulationTab* simTab = new SimulationTab(operation, connector, this);
    operation->setWidget(simTab);
    setCurrentIndex(1);
    job = "simulationTab";
}

void TabWidget::showFromPipeCurveTab(){
    FromPipeCurveTab* fromTab = new FromPipeCurveTab(operation, connector, this);
    operation->setWidget(fromTab);
    setCurrentIndex(1);
    job = "fromPipeCurveTab";
}

bool TabWidget::isOperating(){
    return operation->isSet();
}

void TabWidget::setConnector(GuiConnector *c){
    connector = c;
    tree->setConnector(c);
    operation->setConnector(c);
}


QString TabWidget::doWhat(){
    return job;
}

void TabWidget::showMoveDataTab(){
    MoveDataTab* dataTab = new MoveDataTab(operation, connector, this);
    operation->setWidget(dataTab);
    setCurrentIndex(1);
    job = "moveDataTab";
}
