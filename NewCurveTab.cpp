#include "NewCurveTab.h"
#include <QLabel>
#include <QTextEdit>
#include <QGroupBox>

NewCurveTab::NewCurveTab(QWidget* parent):QWidget(parent)
{
    pointBox = new QGroupBox("Point");
    pointLabel = new QLabel("point",this);
    pointText = new QTextEdit(this);
    dirBox = new QGroupBox("Direction");
    dirLabel = new QLabel("direction",this);
    dirText = new QTextEdit(this);
}

