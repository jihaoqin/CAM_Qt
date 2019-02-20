#include "TabWidget.h"
#include <QDebug>
#include "TabBackground.h"
#include <QLabel>
#include "NewCurveTab.h"

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
    operation->setWidget(newCurve);
    setCurrentIndex(1);
}

