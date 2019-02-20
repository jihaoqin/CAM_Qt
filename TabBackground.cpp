#include "TabBackground.h"
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QDebug>
#include "NewCurveTab.h"
TabBackground::TabBackground(QWidget* parent):QWidget(parent), center(nullptr),layout(nullptr)
{
    layout = new QVBoxLayout(this);
    setLayout(layout);
}

void TabBackground::mouseMoveEvent(QMouseEvent *event){
    event->ignore();
    qDebug()<< "back move";
}

void TabBackground::mousePressEvent(QMouseEvent *event){
    event->ignore();
    qDebug()<< "back press";
}

void TabBackground::setWidget(QWidget *w){
    if(center){
        layout->removeWidget(center);
        delete center;
        center = nullptr;
    }
    else{
        //do nothing
    }
    center = w;
    layout->addWidget(center);
}

bool TabBackground::isSet(){
    if(center == nullptr){
        return false;
    }
    else{
        return true;
    }
}
