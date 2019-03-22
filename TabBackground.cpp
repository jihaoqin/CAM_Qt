#include "TabBackground.h"
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QDebug>
#include "NewCurveTab.h"
#include "GuiConnector.h"

TabBackground::TabBackground(QWidget* parent):QWidget(parent), center(nullptr),layout(nullptr)
{
    layout = new QVBoxLayout(this);
    setLayout(layout);
}

void TabBackground::mouseMoveEvent(QMouseEvent *event){
    event->ignore();
}

void TabBackground::mousePressEvent(QMouseEvent *event){
    event->ignore();
}

void TabBackground::setWidget(QWidget *w){
    if(w){
        layout->removeWidget(center);
        center = w;
        layout->addWidget(center);
    }
    else{
        layout->removeWidget(center);
        center = nullptr;
    }
}

bool TabBackground::isSet(){
    if(center == nullptr){
        return false;
    }
    else{
        return true;
    }
}

void TabBackground::setConnector(GuiConnector *c){
    connector = c;
}
