#include "GenCurveProgressDialog.h"
#include <QVBoxLayout>

GenCurveProgressDialog::GenCurveProgressDialog(int sum, int left, QWidget* parent):QDialog(parent)
{
    Qt::WindowFlags flags = Qt::Dialog;
    flags |= Qt::FramelessWindowHint;
    setWindowFlags(flags);
    sumLabel = new QLabel(this);
    leftLabel = new QLabel(this);
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(sumLabel);
    layout->addWidget(leftLabel);
    setLayout(layout);
    setData(sum, left);
}


void GenCurveProgressDialog::setData(int sum, int left){
    QString sumStr = QString("Total Curve Number:")+ QString::number(sum);
    sumLabel->setText(sumStr);
    QString leftStr = QString("Left Curve Number:")+ QString::number(left);
    leftLabel->setText(leftStr);
    update();
}
