#include "CentralWidget.h"
#include "LeftRibbonLayout.h"
#include <QMouseEvent>
#include <QDebug>
#include <QLabel>
#include <QHBoxLayout>
#include "StrechLabel.h"

CentralWidget::CentralWidget(QWidget *parent) : QWidget(parent), tabWidget(nullptr), glWidget(nullptr), ctrl(nullptr)
{
    setMouseTracking(true);
    tabWidget = new TabWidget(this);
    tabWidget->resize(200,100);
    tabWidget->setMouseTracking(true);
    glWidget = new GLWidget(this);
    glWidget->resize(800, 600);
    tabWidget->raise();
    block = new StrechLabel(this);
    block->resize(10, 30);
    block->raise();
    connect(block, &StrechLabel::strech, this, &CentralWidget::strech);
    if(1){
    LeftRibbonLayout* l = new LeftRibbonLayout;
    l->addBottomWidget(glWidget);
    l->addLeftWidget(tabWidget);
    l->addStrechLabel(block);
    setLayout(l);
    }
    else{
        QHBoxLayout* layout = new QHBoxLayout;
        layout->addWidget(tabWidget);
        layout->addWidget(glWidget);
        setLayout(layout);
    }

    /*
    QLabel *label = new QLabel(this);
    label->setMouseTracking(true);
    label->setText("big");
    label->resize(400,500);
    QHBoxLayout* layout = new QHBoxLayout;
    layout->addWidget(label);
    setLayout(layout);
    */
}


void CentralWidget::bindController(Controller *c){
    glWidget->bindController(c);
    ctrl = c;
}

void CentralWidget::mouseMoveEvent(QMouseEvent *event){
    qDebug()<<"CentralWidget mouseMove";
}

void CentralWidget::mousePressEvent(QMouseEvent *event){
    qDebug()<<"CentralWidget mousePress";
}


void CentralWidget::strech(int deltaX){
    int nextWidth = tabWidget->width() + deltaX;
    if(nextWidth<200){
        //do nothing
    }
    else if(nextWidth > glWidget->width() - block->width()){
        //do nothing
    }
    else{
        tabWidget->resize(nextWidth, height());
    }
}
