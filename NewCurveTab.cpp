#include "NewCurveTab.h"
#include <QLabel>
#include <QTextEdit>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QFormLayout>
#include <QPushButton>

NewCurveTab::NewCurveTab(QWidget* parent):QWidget(parent)
{
    pointBox = new QGroupBox("Point",this);
    pointLabel = new QLabel("point",this);
    pointText = new QTextEdit(this);
    pointText->setFixedHeight(50);
    QFormLayout* layout_1 = new QFormLayout(pointBox);
    layout_1->addWidget(pointLabel);
    layout_1->addWidget(pointText);
    pointBox->setLayout(layout_1);

    dirBox = new QGroupBox("Direction",this);
    dirLabel = new QLabel("direction",this);
    dirText = new QTextEdit(this);
    dirText->setFixedHeight(50);
    QFormLayout* layout_2 = new QFormLayout(dirBox);
    layout_2->addWidget(dirLabel);
    layout_2->addWidget(dirText);
    dirBox->setLayout(layout_2);

    ok = new QPushButton("Ok", this);
    cancle = new QPushButton("Cancle",this);
    QHBoxLayout* layout_3 = new QHBoxLayout();
    layout_3->addWidget(ok);
    layout_3->addWidget(cancle);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(pointBox);
    mainLayout->addWidget(dirBox);
    mainLayout->addLayout(layout_3);
    mainLayout->addStretch(1);
    setLayout(mainLayout);
}


NewCurveTab::~NewCurveTab(){
}

