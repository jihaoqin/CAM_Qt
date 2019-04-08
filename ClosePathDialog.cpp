#include "ClosePathDialog.h"
#include <QLabel>
#include <QVBoxLayout>
ClosePathDialog::ClosePathDialog(QWidget* parent)
    :QDialog (parent)
{
    Qt::WindowFlags flags = Qt::Dialog;
    flags |= Qt::FramelessWindowHint;
    setWindowFlags(flags);
    leftTotal = new QLabel(this);
    leftLeft = new QLabel(this);
    upTotal = new QLabel(this);
    upLeft = new QLabel(this);
    rightTotal = new QLabel(this);
    rightLeft = new QLabel(this);
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(leftTotal);
    layout->addWidget(leftLeft);
    layout->addWidget(upTotal);
    layout->addWidget(upLeft);
    layout->addWidget(rightTotal);
    layout->addWidget(rightLeft);
    setData(-1,-1,-1,-1,-1,-1);
}

void ClosePathDialog::setData(int lt, int ll, int ut, int ul, int rt, int rl){
    QString ltStr = "Left  Total:" + (lt>=0?QString::number(lt):" ");
    QString llStr = "       Left:" + (ll>=0?QString::number(ll):" ");
    QString utStr = "Up    Total:" + (ut>=0?QString::number(ut):" ");
    QString ulStr = "       Left:" + (ul>=0?QString::number(ul):" ");
    QString rtStr = "Right Total:" + (rt>=0?QString::number(rt):" ");
    QString rlStr = "       Left:" + (rl>=0?QString::number(rl):" ");
    leftTotal->setText(ltStr);
    leftLeft->setText(llStr);
    upTotal->setText(utStr);
    upLeft->setText(ulStr);
    rightTotal->setText(rtStr);
    rightLeft->setText(rlStr);
    update();
}
