#include "TabBackground.h"
#include <QMouseEvent>
#include <QDebug>
TabBackground::TabBackground(QWidget* parent):QWidget(parent)
{

}

void TabBackground::mouseMoveEvent(QMouseEvent *event){
    event->ignore();
    qDebug()<< "back move";
}

void TabBackground::mousePressEvent(QMouseEvent *event){
    event->ignore();
    qDebug()<< "back press";
}
