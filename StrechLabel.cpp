#include "StrechLabel.h"
#include <QDebug>
#include <QMouseEvent>

StrechLabel::StrechLabel(QWidget* parent)
    :QLabel(parent)
{
    setMouseTracking(true);
    setStyleSheet("background-color:rgb(255,255,255)");
}

void StrechLabel::enterEvent(QEvent *event){
    setCursor(Qt::SizeHorCursor);
}

void StrechLabel::leaveEvent(QEvent *event){
    setCursor(Qt::ArrowCursor);
}

void StrechLabel::mouseMoveEvent(QMouseEvent *event){
    qDebug()<<"move";
    if(event->buttons()&Qt::LeftButton){
        QPoint nowPos = event->globalPos();
        int deltaX = nowPos.x() - lastPos.x();
        emit strech(deltaX);
        lastPos = nowPos;
    }
}


void StrechLabel::mousePressEvent(QMouseEvent* event){
    lastPos = event->globalPos();
}
