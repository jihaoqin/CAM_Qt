#include "TabWidget.h"
#include <QDebug>
#include "TabBackground.h"
#include <QLabel>

TabWidget::TabWidget(QWidget* parent)
    :QTabWidget(parent),first(nullptr), second(nullptr)
{
    setMouseTracking(true);
    setStyleSheet("background-color:rgb(255,255,255)");
    //setMinimumWidth(200);
    first = new TabBackground(this);
    first->setMouseTracking(true);
    second = new TabBackground(this);
    second->setMouseTracking(true);
    addTab(first, "first");
    addTab(second, "first");
}

