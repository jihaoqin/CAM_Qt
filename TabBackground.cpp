#include "TabBackground.h"
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QDebug>
TabBackground::TabBackground(QWidget* parent):QWidget(parent)
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
    if(!center){
        layout->removeWidget(center);
    }
    else{
        //do nothing
    }
    center = w;
    layout->addWidget(center);
}
